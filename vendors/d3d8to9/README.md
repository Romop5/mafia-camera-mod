d3d8to9
=======

[![Build Status](https://ci.appveyor.com/api/projects/status/aqupdda60ixgenyd?svg=true)](https://ci.appveyor.com/project/crosire/d3d8to9)

This is a pseudo-driver module that intends to improve compatibility and stability in games using Direct3D 8 for rendering by converting all API calls and lowlevel shaders to equivalent Direct3D 9 ones. By that it also opens those games to the new possibilities from proven tools and wrappers written for Direct3D 9.

## Contributing

You'll need Visual Studio 2013 or higher to open the project file. It is recommended to install the old standalone DirectX end-user runtime, which is required for the D3DX libraries used for disassembling and assembling the shaders.

Any contributions to the project are welcomed, it's recommended to use GitHub [pull requests](https://help.github.com/articles/using-pull-requests/).

## [Contributors](https://github.com/crosire/d3d8to9/graphs/contributors)

Thank you to all the contributors making this project possible, especially [elishacloud](https://github.com/elishacloud) for the continuously excellent work improving compatibility with all sorts of games.

## License

All source code in this repository is licensed under a [BSD 2-clause license](LICENSE.md).
