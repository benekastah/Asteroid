# An Asteroid Game

I wanted to learn me some OpenGL and C++, so I decided to build an asteroid
game for myself. It isn't an exact copy of the Atari game. There are a handful
of improvements so far:

- Atari version had difficult controls. In this version, you use the mouse to
  aim and shoot, and WASD or the arrow keys to move. The direction of your
  motion doesn't depend on where you're aiming.
- I didn't make the Atari game, but I did make this one.

I got started with the really nice
[Glitter](http://polytonic.github.io/Glitter/) project, which is a boilerplate
for OpenGL projects. I've since removed unwanted dependencies.

# Roadmap

## Powerups

It should be easy to add a few interesting powerups. Modifying gun parameters
is really simple, and changing player speed as well. A few other powerups I
have in mind:

- Battering ram. For a brief period you can ram asteroids with your ship to
  destroy them instead of the other way around.
- Multiple bullet directions.
- Different bullet types (missiles that seek targets, mines that stay in one
  place and destroy asteroids on contact, etc.)

## Crafting

It would be interesting if you were a space miner of sorts. You could collect
raw materials from asteroids you destroy and use them to craft upgrades and
powerups.

## Health

Instead of dying any time you are struck by an asteroid, have the asteroid
decrease your health relative to the force it hit you with. This should
damage the asteroid as well.

## Fuel gauge

You have a limited amount of fuel. How fast you can go depends on your engine
upgrade and how much stuff you are carrying. To decrease your weight, you have
to offload at a station of some kind.
