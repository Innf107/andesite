#EXPECT: 3
scoreboard objectives add test dummy
scoreboard players set A test 8
scoreboard players remove A test 5
scoreboard players get A test
