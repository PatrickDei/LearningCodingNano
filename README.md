guidelines:
* code formating -> Clang Format
 --> xcode (ttps://github.com/mapbox/XcodeClangFormat)
 --> clang format file saved in repository (.clang-format)
 --> follow the coding style used in the project
* project repository / xcode-project-file hierarchy
 --> they are mostly identical and are to be maintained as such
 --> after adding resources of any kind to project file apply folder sorting by name
* each new developer should make a new branch for the learning tasks
* android -> install AndroidStudio and set it up -> don't upgrade gradle if you don't know what you're doing -> the code base is mostly the same
* menus are created with NanoEditor
 --> use old menus as a base for new ones
 --> use Custom device resolution: 750 x 1272 (iphone X without notch, header and footer)
 --> enable 'on copy save names'(recommended copy a old menu -> to create a new -> delete root node and start from there)
 --> create root node as parent node for menu with relative position (0.5,0.5)
 --> use (0.5,0.5) as default anchor point
 --> use relative positions as default - with offset as necessary 
