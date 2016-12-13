# bbtree
This tool allows to save big B&amp;B tree (>100 millions nodes), to do tree analysis (tree balancing) and to visualize it.

1) to compile call!!!!!
g++ -std=c++0x bbtree.cpp

2) buf.data - format data sample from client or tree simulator. Note format is text file with fixed length fields.

TIME        |ID NODE     |ID PARENT   |ID_BRANCH|STATE|INFO       
04:07:25:123|           1|           0| 1       |3    |Root node  
04:07:25:234|           2|           1| 1       |3    |Some node  
