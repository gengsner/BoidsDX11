## About
3D boid simulation in in C++ using compute shaders with the DX11 API.<br/> 
The graphics engine was built as an assignment in the first year of school at The Game Assembly.   

Blog post here: http://gengsner.github.io/posts/boids/

<br/> 

## Build
To play with the simulation:
1. Generate a Visual Studio project by running _generate_sln.bat_. 
2. Build the Visual Studio project to create an executable in _root/bin/_.

Window size and fullscreen can be configured in _root/bin/windowSettings.json_.<br/>
Requires Microsoft Direct3D feature level 11.

<br/>

## Controls

### Camera mode
| Action        | Key(s)               |
|---------------|----------------------|
| Move          | `W`, `S`, `A`, `D`   |
| Move up/down  | `SPACE` / `CTRL`     |
| Rotate        | `RMB` + `MOVE MOUSE` |
| Adjust Speed  | `SCROLL WHEEL`       |
| Hide UI       | `ESC`                |

### Player
| Action                | Key(s)       |
|-----------------------|--------------|
| Hide UI to control    | `ESC`        |
| Adjust Speed          | `W`, `S`     |
| Roll                  | `A`, `D`     |
| Pitch, Yaw            | `MOVE MOUSE` |

<br/>

## Settings

Simulation settings can be saved and loaded in runtime.<br/> 
**Note** that saving will overrite the previously saved settings.

### Defined limits
Will halt the simulation of exceeded. 
| Parameter             | Max Value     |
|-----------------------|---------------|
| Max boid count        | `10,000,000`  |
| Max grid cell count  | `100,000,000` |
| Max boids/ cell       | `50,000`      |

### Modifiable Parameters
| General settings |
|------------------|
| Time Scale       |

Time Scale multiplies the actual delta time for the simualtion. Will halt the simulation if at '0'.<br/>

| Boid settings    |
|------------------|
| Boid Count       |
| Cohesion Factor  |
| Separation Factor|
| Alignment Factor |
| Visual Range     |
| Protected Range <br/>*(Boids separate within this range. Needs to be <= Visual Range)*|
| Field of View    | 
| Max Speed        |
| Min Speed        |
| Gravity          |


| Grid settings  |
|----------------|
| Gridding On    |
| Cell Size Mult |
| Min Position   |
| Max Position   |
| Turn Speed<br/>*(Max speed for avoiding to leave the grid)*|
| Turn Margin <br/>*(Distance within which Boids will start avoiding to leave the grid)*|

The size of the grid will be Max Position - Min Position.<br/>
The size of cells will be Visual Range * Cell Size Mult.

<br/>

| Graphics settings            |
|------------------------------|
| Render Bounds<br/>*(Render the bounds of the grid)*|
| Directional Light Direction  |
| Directional Light Color      |
| Directional Light Intensity  |
| Ambient Light Color          |  
| Ambient Light Intensity      |
| Boid Color                   |
| Boid Alt Color               |
| Flock Size to Fully Color<br/>*(Gradually shifts boid's color to Boid Alt Color as flock size reaches this number)*|
| Bounds Color                 |
| Player Color                 |
| Clear Color<br/>*(Color of space where there are no objects)*|
| View Dist<br/>*(Gradually fades out objects to Clear Color as distance from camera reaches this number)*|

| Player settings   |
|-------------------|
| Max Velocity      |
| Acceleration      |
| Roll Acceleration |
| Mouse Sensitivity |
| Boid Attraction<br/>*(Controls how other boids iteract with the Player. Negative values will have boids avoiding.)*|


