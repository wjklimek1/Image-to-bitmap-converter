# Image-to-bitmap-converter
This piece of software allows you to convert image to C header file with array of bytes. Such array can be displayed as a bitmap on embedded LCD driven by microcontroller like Arduino or STM32.

Currently, only source code is provided. To compile this project in QtCreator you will need Qt5 and OpenCV.

To convert image, just select "Open image" button and select image file from your file system. Supported file formats are .png, .jpg and .bmp. Image preview will be displayed in a main window of appliccation. Then specify array name in a textbox. Keep in mind that it can't start with a number and can contain only ASCII letters. To export 16-bit color array in RGB565 format, click "Export.h file" and save header file. 

You can also reverse bytes order in this 16bit values by checking "reverse bytes order". Originally, representation of RGB565 bits is following: RRRRRGGG GGGBBBBB. However, for my [STM32 DMA library](https://github.com/wjklimek1/ILI9341_DMA_library) i needed to place LSB first, to get: GGGBBBBB RRRRRGGG. This is what "reverse bytes order" does.

You can create also a binary bitmap, but currently it is exported as RGB565 array containing only white and black pixels. This feature will be developded in a future.
