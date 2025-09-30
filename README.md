# ENCODING SECRET MESSAGES IN IMAGE FILES IN C++

## Overview
The goal of this project was to create a program capable of encoding secret messages in image files in a way that does not visibly alter the image itself. The is achieved by modifying the least significant bit of color/alpha values of individual pixels. Accesssing and modifying the pixel values is handled by [SFML](https://github.com/SFML/SFML "Simple and Fast Multimedia Library").

## Table of Contents
- [Detailed Description](#detailed-description)
- [Project Structure](#project-structure)
- [Features](#features)
- [Preview of The Application](#preview-of-the-application)
- [Technologies Used](#technologies-used)
- [Implementation Details](#implementation-details)
- [Future Work](#future-work)

## Detailed Description
The fundamental idea this project is based on is manipulating the least significant bit (LSB) to store information in color values of the pixels of image files. By modifying the LSB, a secret message can be encoded into the picture, while simultaneously altering the image itself in such a small way, it is virtually impossible to see with a naked eye. That way, as long as an unwated third party isn't aware the message had been encoded in the image file, there is no way for them to retrieve any information from the picture just by looking at it. Using this technique, this program is able to encode a secret text message and store it inside a picture. It is also possible to then retrieve the message by collecting the LSBs of the pixels and putting them together to recreate the input string. A limitation of this approach's implementation is that it only works for UTF-8 characters. The reason for that is because the way the program encodes text in the image files is by splitting every character's bit representation and replacing the LSB of the rbga values of designated pixels with each bit derived from the singular UTF-8 character. Any UTF-8 character is represented by an 8-bit value - for example, the upppercase letter 'A' has a binary representation of '01000001'.  

The stages of the project were:


## Project Structure

## Features

### Preview of The Application

\
<img src="" alt="" width="400"/>


## Technologies Used

## Implementation Details

## Future Work
