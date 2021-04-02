# Base64 CLI

![MSBuild](https://github.com/tsmoreland/base64.cpp/workflows/MSBuild/badge.svg)

Simple command line utility intended to take a file source and encode to or decode from Base64

The general idea is to eventually support the following usage:

- ```<command> encode (input file) (output file)``` 
- ```<command> decode (input file) (output file)```

where ```(input file)``` and ```(output file)``` are optional, 
if output file isn't provided the write to stdout and maybe copy to clipboard.
If input file isn't provided then either read from stdin or may clipboard.

It's likely there will be minimal checks at best for file size, it is attempting to stream for files so that may work out but for anything else the file is read in chunks but the encoded or decoded result will be in memory

## Remarks

As I've already done this project in Java and will likely do another in C# this implementation serves mostly as an experiment with C++20 features with primary focus on 

- Concepts
- std::span
- std::format (or fmt::format for which serves as the basis of std::format - not yet available in msvc)

Earlier attempts were made to use modules but they aren't really far enough along to use and it's still not very clear hwo if at all they fit into a shared or static library.  I had some limited success with static libraries but none with shared
