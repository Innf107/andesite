#EXPECT: 1
scoreboard objectives add objA dummy
scoreboard objectives add objB dummy

scoreboard players set A objA 3
scoreboard players operation B objB = A objA
scoreboard players operation A objA += B objB
scoreboard players set B objB 5
scoreboard players operation A objA *= B objB
scoreboard players set B objB 4
scoreboard players operation A objA %= B objB
scoreboard players set B objB 2
scoreboard players operation A objA /= B objB
scoreboard players set B objB 5
scoreboard players operation A objA >< B objB
scoreboard players operation A objA < B objB
scoreboard players set B objB 0
scoreboard players operation A objA > B objB
scoreboard players get A objA
