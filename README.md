<div align="center"> 

  <h3>Pac-Man</h3>

  <img width="32" src="https://i.imgur.com/oco22UU.png">
  <img width="32" src="https://i.imgur.com/iEdyLHW.png">
  <img width="32" src="https://i.imgur.com/iEdyLHW.png">
  <img width="32" src="https://i.imgur.com/E4WfkDu.png">
</div>

## Description

This project have been made for me to learn `C` and `SDL2` library.

## Usage

To compile the project, you need to have `SDL2` and `SDL2_image` installed on your computer.

Then, you can compile the project with the following command:

```bash
cd src
make
```

This will create a `pacman` executable in the `bin` folder.

To run the game, you can use the following command:

```bash
cd bin
chmod +x pacman
./pacman
```

To clean the project, you can use the following command:

```bash
cd src
make clean
```

This will remove all the `.o` files and the `pacman` executable.

## Features

* Custom level file format (see <a href="#leveling">Leveling</a>)
* Custom tileset file format (see [tileset.png](assets/textures/tileset.png))
* Custom font file format (see [font.ttf](assets/font.ttf)) (note that the default font is Open Sans Regular from Google Fonts)
* Sprite animation (ghosts and pacman) :
  * Ghosts (see <a href="#ghosts">Ghosts</a>)
  * Pacman (see <a href="#pacman">Pacman</a>)
* Ghost AI (Blinky, Pinky, Inky and Clyde)
* Bonus (cherry, strawberry, orange, apple) (see <a href="#bonus">Bonus</a>)

## Leveling

You can create your own level by editing `level.txt` file in the `data` folder.

All the tiles are defined in the [tileset.png](assets/textures/tileset.png) file.

Here is the list of all the tiles:

> Note that the tileset is composed of 32x32 tiles.

> Note also that you have multiple walls types (see [tileset.png](assets/textures/tileset.png)). You have a wall type that is taken the 2/3 of the tile and another one that is taken the 1/3 of the tile, the last one is centered in the tile.

| Tile | Description | Character |
| --- | --- | --- |
| Accessible tiles |
| <img src="https://i.imgur.com/091GqiI.png" width="64"> | Empty tile | `0` |
| <img src="https://i.imgur.com/iEdyLHW.png" width="64"> | Dot tile | `.` |
| <img src="https://i.imgur.com/LDMPyh5.png" width="64"> | Big dot tile | `p` |
| <img src="https://i.imgur.com/nM7ca2b.png" width="64"> | Wall full tile | `#` |
| 2/3 wall tiles |
| <img src="https://i.imgur.com/r9jQCoV.png" width="64"> | Wall tile down | `d` |
| <img src="https://i.imgur.com/X5ED23A.png" width="64"> | Wall tile up | `u` |
| <img src="https://i.imgur.com/reBswvw.png" width="64"> | Wall tile left | `l` |
| <img src="https://i.imgur.com/yptHzMh.png" width="64"> | Wall tile right | `r` |
| 1/3 wall tiles |
| <img src="https://i.imgur.com/ISlvAML.png" width="64"> | Horizontal tile | `=` |
| <img src="https://i.imgur.com/0EMQetE.png" width="64"> | Horizontal tile left | `<` |
| <img src="https://i.imgur.com/Wvgeya5.png" width="64"> | Horizontal tile right | `>` |
| <img src="https://i.imgur.com/GYx6Hx4.png" width="64"> | Vertical tile | `|` |
| <img src="https://i.imgur.com/EFSpK8d.png" width="64"> | Vertical tile up | `^` |
| <img src="https://i.imgur.com/WaS5LMg.png" width="64"> | Vertical tile down | `v` |
| 2/3 Corner tiles |
| <img src="https://i.imgur.com/tjjJmZ5.png" width="64"> | Bottom left corner tile | `1` |
| <img src="https://i.imgur.com/LAaPRD1.png" width="64"> | Bottom right corner tile | `2` |
| <img src="https://i.imgur.com/MtHYGpO.png" width="64"> | Top left corner tile | `3` |
| <img src="https://i.imgur.com/dTjhpxx.png" width="64"> | Top right corner tile | `4` |
| 1/3 Corner tiles |
| <img src="https://i.imgur.com/WZ2IVxO.png" width="64"> | Bottom left corner tile | `[` |
| <img src="https://i.imgur.com/vCRiXnz.png" width="64"> | Bottom right corner tile | `]` |
| <img src="https://i.imgur.com/61xeBoF.png" width="64"> | Top left corner tile | `(` |
| <img src="https://i.imgur.com/bVayz86.png" width="64"> | Top right corner tile | `)` |
| 1/3 T tiles |
| <img src="https://i.imgur.com/lemlalL.png" width="64"> | T tile down | `s` |
| <img src="https://i.imgur.com/fd1T74H.png" width="64"> | T tile up | `z` |
| <img src="https://i.imgur.com/G8efHLJ.png" width="64"> | T tile left | `q` |
| <img src="https://i.imgur.com/QEe1qoL.png" width="64"> | T tile right | `f` |
| Cross tiles |
| <img src="https://i.imgur.com/7o06EIn.png" width="64"> | Cross tile | `+` |

## Ghosts

The ghosts are controlled by a random algorithm. The ghosts can move in 4 directions (up, down, left and right). The ghosts can't move in the opposite direction of their current direction.

| Ghost | Color | Sprite |
| --- | --- | --- |
| Blinky | Red | ![Ghost](assets/sprites/ghost_3.png) |
| Pinky | Pink | ![Ghost](assets/sprites/ghost_2.png) |
| Inky | Cyan | ![Ghost](assets/sprites/ghost_1.png) |
| Clyde | Orange | ![Ghost](assets/sprites/ghost_4.png) |
| Scared ghost | White / Blue | ![Ghost](assets/sprites/ghost_scared.png) |

## Bonus

The bonus will appear at a random interval of time and have a random position on the map. The bonus will disappear after a certain amount of time. Then the bonus will automatically reappear at a random interval of time and have a random position on the map.

![Bonus](assets/sprites/bonus.png)

## Pacman

Pacman is controlled by the player. The player can move in 4 directions (up, down, left and right).

The player goal is to eat all the dots and the bonus. The player can eat the ghosts after eating a big dot (the ghosts will be scared for a certain amount of time).

![Pacman](assets/sprites/pacman2.png)

## Controls

| Key | Action |
| --- | --- |
| `arrow up` | Move up |
| `arrow down` | Move down |
| `arrow left` | Move left |
| `arrow right` | Move right |
| `space` | Start the game |
| `escape` | Pause the game |
| `lctrl + f` | Toggle FPS |
| `lalt + f4` | Quit the game |
| `lctrl + r` | Restart the game |
| `enter` | Validate action |

## Screenshots

![Screenshot 1](https://i.imgur.com/HzC7cHy.png)
> Start screen

![Screenshot 2](https://i.imgur.com/sEYOJXt.png)
> Game screen

![Screenshot 3](https://i.imgur.com/cOM7m0p.png)
> Pause screen

![Screenshot 4](https://i.imgur.com/fAtddkA.png)
> Ghosts scared

![Screenshot 5](https://i.imgur.com/ljKPrFL.png)
> Bonus

![Screenshot 6](https://i.imgur.com/6WgnHMY.png)
> Game over screen

## Contributing

Feel free to contribute to this project by forking it and making a pull request.

If you find any bug or have any suggestion to improve the project, please open an issue.

You will find a list of things that I would like to add or fix in the [TODO](TODO.md) file.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Author

* **[Hokanosekai](https://gihub.com/hokanosekai)** - *Initial work*

## Acknowledgements

* [SDL2](https://www.libsdl.org/)
* [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
* [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)