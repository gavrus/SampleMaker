SampleMaker
===========
Qt Image cropper

Utility with GUI, that allows you cut regions of interest from images, replicate  them with adding some variations and save the resulting images to files. 
This can be useful for creating datsets for machine learning tasks in computer vision: training convolution neural networks, haar cascades, etc.

BUILD process is very straightforward: double click on SampleMaker.pro file and build project in Qt Creator. NOTE: you must have installed Qt libraries and Qt creator. 


HOW TO USE THIS TOOL?
First of all, you must to specify work directories: Source folder - where the image source files are stored, Target folder - where the cropped images will be saved.
Image cropping treats by blue rectangle that is movable and rotatable. Left mouse button performs drag and drop, right mouse button and mouse wheel performs rotation. In case which cursor places on image, mouse wheel performs picture's scaling.
