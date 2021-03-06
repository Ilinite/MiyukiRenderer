//
// Created by Shiina Miyuki on 2019/3/10.
//

#include "texture.h"

namespace Miyuki {

    std::shared_ptr<IO::Image> ImageLoader::load(const std::string &filename, IO::ImageFormat format) {
        auto file = cxx::filesystem::path(filename);
        std::string fullPath = cxx::filesystem::absolute(file).string();
        auto temp = fullPath;
        fullPath.clear();
        for (auto c: temp) {
            fullPath += c == '\\' ? '/' : c;
        }

        if (images.find(filename) != images.end()) {
            return images[filename];
        }
        fmt::print("Loading {}\n", fullPath);
        auto ptr = std::make_shared<IO::Image>(filename, format);
        return ptr;
    }

    Spectrum Texture::evalUV(const Point2f &uv) const {
        if (!image) {
            return evalAlbedo();
        } else {
            int x = static_cast<int>(uv.x() * image->width);
            int y = static_cast<int>((1 - uv.y()) * image->height);
            return evalAlbedo() * image->operator()(x, y);
        }
    }

    Spectrum Texture::evalPixel(const Point2i &pos) const {
        if (!image) {
            return evalAlbedo();
        } else {
            return evalAlbedo() * image->operator()(pos.x(), pos.y());
        }
    }
}