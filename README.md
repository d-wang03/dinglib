# dinglib
The Ding Library is a general middle-ware framework, which can support Common API in Genivi.

It creates a Qt-like signal-slot mechanism. 
Different objects inherited from ding::DObject, can declear signals, and connect them to any member functions. 

This is like the new connect in Qt, and have the adventure for type examination at compile time.
