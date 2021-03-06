//
// Created by Shiina Miyuki on 2019/3/9.
//

#include "image.h"

#define STB_IMAGE_IMPLEMENTATION

#include "thirdparty/stb/stb_image.h"
#include <thirdparty/stb/stb_image_write.h>
#include <utils/thread.h>

namespace Miyuki {
    namespace IO {

        Image::Image(const std::string &filename, ImageFormat format)
        : GenericImage<Spectrum>(), format(format), filename(filename) {
            int ch;
            auto data = stbi_load(filename.c_str(), &width, &height, &ch, 3);
            if (!data) {
                throw std::runtime_error(fmt::format("Cannot load {}\n", filename).c_str());
            }
            std::function<Float(uint8_t)> f;
            if (format == ImageFormat::none) {
                f = [](uint8_t _x) -> Float {
                    double x = _x / 255.0f;
                    return std::pow(x, 2.2);
                };
            } else if (format == ImageFormat::raw) {
                f = [](uint8_t _x) -> Float {
                    return _x / 255.0;
                };
            }
            pixelData.resize(width * height);
            Thread::ParallelFor(0u, width * height, [&](uint32_t i, uint32_t threadId) {
                pixelData[i] = Spectrum(f(data[3 * i]),
                                        f(data[3 * i + 1]),
                                        f(data[3 * i + 2]));
            }, 1024);
        }

        void Image::save(const std::string &filename) {
            std::vector<unsigned char> pixelBuffer;
            for (const auto &i:pixelData) {
                auto out = i.gammaCorrection();
                pixelBuffer.emplace_back(out.r());
                pixelBuffer.emplace_back(out.g());
                pixelBuffer.emplace_back(out.b());
                pixelBuffer.emplace_back(255);
            }
            lodepng::encode(filename, pixelBuffer, (uint32_t) width, (uint32_t) height);
        }
    }
}