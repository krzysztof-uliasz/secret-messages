#include <vector>
#include <fmt/base.h>
#include <fmt/ranges.h>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics.hpp>
#include <bitset>
#include <iostream>

auto encodeOneCharacterToVector(const std::bitset<8> &charAsBitset, std::vector<int> &combinedPixelDataVector, const int &startingPoint) -> void;

auto createVectorWithPixelData(const sf::Image& image, const unsigned &neededPixelRows) -> std::vector<int>;

auto saveImageWithSecretMessage(const std::vector<int>& vector, sf::Image& image) -> void;

auto encodeSecretMessageToVector(const std::string& secretMessage, const sf::Image& image, const std::string& key) -> std::vector<int>;

auto encodeSecretMessage(const std::string& secretMessage, const std::string& key, sf::Image& image, const std::string& path) -> void;

auto encodeSizeToVector(const std::bitset<32> &intAsBitset, std::vector<int> &combinedPixelDataVector, const int &startingPoint) -> void;

auto calcLengthOfSecretMessage(const std::vector<int>& vector, int keyLengthInBits) -> int;

auto decodeMessage(const std::string& key, const sf::Image& image, int bitsForMessageLength) -> std::string;

auto main() -> int {

    auto image = sf::Image("test2.png");
    const auto secretMessage = std::string("secret message test");
    const auto key = std::string("key");
    constexpr auto bitsForMessageLength = 32;
    fmt::println("secret message: {}", secretMessage);

    auto path = std::string("output1.png");
    //encodeSecretMessage(secretMessage, key, image, path);

    fmt::println("message retrieved: {}", decodeMessage(key, sf::Image(path), bitsForMessageLength));

    return 0;
}

auto encodeOneCharacterToVector(const std::bitset<8> &charAsBitset, std::vector<int> &combinedPixelDataVector, const int &startingPoint) -> void {

    for (int i = 0; i < charAsBitset.size(); i++) {
        int lewa = 1 & charAsBitset[i];
        int prawa = 1 & combinedPixelDataVector[i + startingPoint];
        if (lewa != prawa) {
            switch (lewa) {
                case 0: combinedPixelDataVector[i + startingPoint]--;
                break;
                case 1: combinedPixelDataVector[i + startingPoint]++;
                break;
                default:
                    fmt::println("fatal error...");
                throw std::exception();
            }
        }
    }
}

auto encodeSizeToVector(const std::bitset<32> &intAsBitset, std::vector<int> &combinedPixelDataVector, const int &startingPoint) -> void {

    for (int i = 0; i < intAsBitset.size(); i++) {
        int lewa = 1 & intAsBitset[i]; //intAsBitset.size() - i - 1
        int prawa = 1 & combinedPixelDataVector[i + startingPoint];
        if (lewa != prawa) {
            switch (lewa) {
                case 0: combinedPixelDataVector[i + startingPoint]--;
                break;
                case 1: combinedPixelDataVector[i + startingPoint]++;
                break;
                default:
                    fmt::println("fatal error...");
                throw std::exception();
            }
        }
    }
}

auto createVectorWithPixelData(const sf::Image& image, const unsigned &neededPixelRows) -> std::vector<int> {

    auto vector = std::vector<int>();

    for (unsigned x = 0; x < image.getSize().x; x++) {
        for (unsigned y = 0; y < neededPixelRows; y++) {
            auto colorValue = image.getPixel({x, y});
            vector.push_back(colorValue.r);
            vector.push_back(colorValue.g);
            vector.push_back(colorValue.b);
            vector.push_back(colorValue.a);
        }
    }

    return vector;
}

auto saveImageWithSecretMessage(const std::vector<int>& vector, sf::Image& image, const std::string& path) -> void {

    // fmt::println("sekretna wiadomość:");
    // fmt::println("{}", vector);

    for (int i = 0; i < vector.size(); i += 4) {
        auto newColor = sf::Color(vector[i], vector[i + 1], vector[i + 2], vector[i + 3]); //

        auto pixelNumber = i / 4;
        sf::Vector2u pixelCoordinates = {pixelNumber % image.getSize().x, pixelNumber / image.getSize().x};
        image.setPixel(pixelCoordinates, newColor);
    }

    if (!image.saveToFile(path)) fmt::println("Error >:(\n");


}

auto encodeSecretMessageToVector(const std::string& secretMessage, const sf::Image& image, const std::string& key) -> std::vector<int> {

    const auto fullMessageSize = secretMessage.size() + key.size() + 4;

    auto neededPixelRows = 2 * fullMessageSize / image.getSize().x;

    if (fullMessageSize % image.getSize().x  != 0) { neededPixelRows++; }
    auto vectorWithPixelData = createVectorWithPixelData(image, neededPixelRows);

    //fmt::println("pobrane wartości pixeli");
    //fmt::println("{}", vectorWithPixelData);

    auto startingPoint = 0;

    //encode key
    for (int i = 0; i < key.size(); i++) {
        auto charInBinaryForm = std::bitset<8>(key[i]);
        encodeOneCharacterToVector(charInBinaryForm, vectorWithPixelData, startingPoint);
        startingPoint += 8;
    }

    //encode size
    const auto encodedMessageSize = std::bitset<32>(secretMessage.size());
    encodeSizeToVector(encodedMessageSize, vectorWithPixelData, startingPoint);
    startingPoint += 32;

    //encode message
    for (int i = 0; i < secretMessage.size(); i++) {
        auto charInBinaryForm = std::bitset<8>(secretMessage[i]);
        encodeOneCharacterToVector(charInBinaryForm, vectorWithPixelData, startingPoint);
        startingPoint += 8;
    }

    // fmt::println("secret message combined with color values: ");
    // fmt::println("{}", vectorWithPixelData);
    return vectorWithPixelData;

}

auto encodeSecretMessage(const std::string& secretMessage, const std::string& key, sf::Image& image, const std::string& path) -> void {

    constexpr auto messageLengthAsIntInPixels = 4;
    const auto finalLength = secretMessage.size() + key.size() + messageLengthAsIntInPixels;

    //every character needs 2 pixels
    //5 * 2 pixels to store a special key
    //4 * 2 pixels used to store message length
    if (finalLength > image.getSize().x * image.getSize().y / 2) {
        fmt::println(
            "Plik jest za mały, aby enkodować tą wiadomość. Potrzebujesz przynajmniej {} pikseli. Ten plik ma tylko {} pikseli.",
            finalLength * 2,
            image.getSize().x * image.getSize().y
            );
    }

    else {
        auto vectorWithEncodedMessage = encodeSecretMessageToVector(secretMessage, image, key);
        saveImageWithSecretMessage(vectorWithEncodedMessage, image, path);
        fmt::println("Message encoded!");
        //fmt::println("Wiadomość zakodowana!");
    }
}

auto calcLengthOfSecretMessage(const std::vector<int>& vector, const int keyLengthInBits) -> int {

    auto number = std::bitset<32>();
    for (int i = keyLengthInBits; i < keyLengthInBits + 32; i++) {
        number[i - keyLengthInBits] = vector[i];
    }
    return number.to_ulong();
}

auto decodeMessage(const std::string& key, const sf::Image& image, int bitsForMessageLength) -> std::string {

    fmt::println("decoding...");
    auto bitsForKeyAndLength = bitsForMessageLength + key.size() * 8;

    auto vectorPom = std::vector<int>();
    for (unsigned i = 0; i <= bitsForKeyAndLength / image.getSize().x; i++) {
        for (unsigned j = 0; j < image.getSize().x; j++) {
            if (i < bitsForKeyAndLength / image.getSize().x || j < bitsForKeyAndLength % image.getSize().x) {
                auto colorValue = image.getPixel({j, i});
                vectorPom.push_back(1 & colorValue.r);
                vectorPom.push_back(1 & colorValue.g);
                vectorPom.push_back(1 & colorValue.b);
                vectorPom.push_back(1 & colorValue.a);
            }
        }
    }

    auto messageLength = calcLengthOfSecretMessage(vectorPom, key.size() * 8);
    //fmt::println("length of the hidden message: {}", messageLength);

    auto temp = bitsForKeyAndLength + messageLength * 8;
    auto vector1 = std::vector<int>();
    for (unsigned i = 0; i <= temp / image.getSize().x; i++) {
        for (unsigned j = 0; j < image.getSize().x; j++) {
            if (i < temp / image.getSize().x || j < temp % image.getSize().x) {
                auto colorValue = image.getPixel({j, i});
                vector1.push_back(1 & colorValue.r);
                vector1.push_back(1 & colorValue.g);
                vector1.push_back(1 & colorValue.b);
                vector1.push_back(1 & colorValue.a);
            }
        }
    }

    auto vector2 = std::vector<char>();
    for (int i = bitsForKeyAndLength; i < temp; i += 8) {
        auto znak = std::bitset<8>();
        for (int j = 0; j < 8; j++) {
            znak[j] = vector1[i + j];
        }
        auto pom1 = znak.to_ulong();
        auto character = static_cast<unsigned char>(pom1);
        vector2.push_back(character);
    }
    //fmt::println("{}", vector2);
    auto tekst = std::string();
    for (auto znak : vector2) {
        tekst += znak;
    }

    return tekst;
}