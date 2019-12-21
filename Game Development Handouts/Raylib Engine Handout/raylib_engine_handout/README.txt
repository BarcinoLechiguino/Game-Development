
Game Development Handout ported from SDL to raylib (www.raylib.com) by Ramon Santamaria (@raysan5)

This handout is intended to be used as a reference for future projects.

Some of the changes to SDL version:

 - j1Window review: Now stores all window-related information, including flags and window events.
 - j1Renderer review: Improved to use raylib internal batch system, added Draw() and DrawText() methods.  
 - j1PerfTimer removed: Now j1Timer uses raylib GetTime() function, already a high-performance timer (if available).
 - j1Textures removed: Not required any more, using Texture2D struct for texture where required.
 - j1Fonts simplified: Just storing and managing one Font. It can be improved.
 - j1Audio reviewed: Now it uses raylib audio system.
 - VS Project: Classes and folders reorganized for consistency
 - VS Project: Created a new project for raylib, with sources and compiled with main project.
 - VS Project: Improved build configuration to support building raylib as static (.lib) and dynamic (.dll) library.
 - VS Project: Configured output directories for intermediate objects (obj/) and binary (Game/)

Check the code and have fun! :)
 