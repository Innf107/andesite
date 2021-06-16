#FLAGS: -XRelaxedWhitespace
#EXPECT: 42
scoreboard   objectives     add test dummy  
scoreboard players   set A test 42  # Comment
scoreboard    players   get A test   #   Comment
