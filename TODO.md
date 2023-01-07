## TODO

Here is a list of things that I would like to add or some known bugs that I would like to fix:

* [x] Fix the left top corner tile that is not displayed

* [ ] Add all others bonus (melon, galaxian, bell, key)
* [ ] Fix the best score :
  * [x] Load the best score from a file
  * [ ] Save the best score in a file
  * [ ] Display the best score in the game

* [x] Fix crash on game running :

> Don't forget to Destroy Texture and Surface after create them for a text display

```c
  window.c 

  l:  190 SDL_DestroyTexture(text);
  l:  191 SDL_FreeSurface(surface);
```
