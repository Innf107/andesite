#EXPECT:Can't get value of test for A; none is set
scoreboard objectives add test dummy
scoreboard players set A test 3
scoreboard players reset A test
scoreboard players get A test
