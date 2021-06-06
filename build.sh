#!/bin/bash
g++ browser.cc -o browser `pkg-config gtkmm-3.0 --libs --cflags` `pkg-config webkit2gtk-4.0 --libs --cflags` -std=c++11 && ./browser
