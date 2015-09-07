#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-

stdout = []
skylineout = []

def format(filename, content):
    with open(filename, "r+") as answer:
        for line in answer:
            content.append(line)
    content.sort()
    with open(filename, "w+") as output:
        for line in content:
            output.write(line);

def main():
    content = []
    format('stdout.txt', content)
    stdout = content

    content = []
    format('skylineout.txt', content)
    skylineout = content

    if cmp(stdout, skylineout) == 0:
        print "Matched~~~~~~~~~~~~~~~~~~"
    else:
        print "Wrong!!!!!!!!!!!!!!!!!!!!"

if __name__ == '__main__':
    main()
