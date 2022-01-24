# Copium

Shuprovo Sikder and Geoffrey Lin  
Period 4

### Description
Our project is a music library that interacts with mp3s in order to play music. 

### List of Required Libraries
- FFmpeg
	`apt-get install libavformat-dev libavutil-dev libavcodec-dev`
- SDL
	`apt-get install libsdl2-dev libsdl2-mixer-dev`
- ncurses
    `apt-get install libncurses5-dev`

### How to Use
- Compile using `make`
- Run using `./copium`. When opened it will read a libraries.data file or if you don't one it will run and prompt you for a mp3 file located in the folder. Enter a mp3 file name and it will add it to the library. If you want to add more mp3 files to the library, use the 'a' key.
- Navigate through your albums using the up or down arrow keys. In order to play the music, go any of the albums and use the right arrow key which will bring you to the tracks column and hit enter. You can skip a track by using the '>' key.
- If you want to quit use the 'q'  key and if you want to save your library data use the 'w' key.

-------------------------------------
# Proposal

### Description
Our project is to create a music library that can interact with mp3 files in order to play music while using the library.

### User Interface
There will be a GUI with an album list to the left. Once the user clicks on an album, the tracks of the album will be shown in the middle of the program. There will be multiple functions depending on how the user interacts with the track (add to queue, clear queue and play, display track info, etc.) To the right, track info will be displayed like album, artist, duration, and any other metadata we find. If we have time, if the track has an image embedded, we could also display that. At the bottom, there will be a progress bar with the current playing track title and various buttons that do different functions like saving the music library, etc. Rough sketch below:

![](https://i.imgur.com/rAAnkCw.png)

### Technical Design
- Topics
  - Allocating memory: Memory needed for data structures
  - Working with files & finding information about files: The user can choose to open an mp3 file in the GUI and then the program will get the METADATA from the file and also play the music.
  - Processes: The program will have to fork and execute the functions such as playing mp3, adding to the library, etc.
  - Signals: Different keybindings will do different functions (i.e. playback, pause, next, forward, etc.)
- Breakdown
  - Shuprovo
    - GUI
      - We plan to use GTK for the GUI and details are listed above
    - Sound
      - When the user selects a track it will play the audio of the mp3. We plan to use SDL for audio.
    - Sorting
      - There will be a button that sorts the list of tracks by their METADATA
  - Geoffrey
    - METADATA processing
      - Getting information about the song from the mp3 file in order to add it to a linked list. Also maybe getting the image from the mp3 if there is one and we have time.
    - Signals
      - Implementation of different hotkeys for the music player using signals
    - Saving library data
      - Saving the linked list of track structs as a file. The track structs will have a variable for the path of the file so the list can be opened later.
- Algorithms / Data Structures
  - Track Struct
    - Name
    - Artist
    - Album
    - Duration
    - Path
  - Linked List of track structs & song structs
    - Linked list so that the music player knows which to play next
    - Linked list of all tracks to save as library data
  - Insertion sort for sorting tracks by METADATA

### Timeline
1/13: Working sound demo  
1/14: Completion of METADATA processing  
1/15: Hopefully completed GUI  
1/17: Saving of library data and opening of it  
1/18: Sorting library data  
1/20: Signal hotkeys  
