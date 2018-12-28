DevMax Path Library Example
===========================

This repository illustrates a real-world software implementation example of a file|directory path processing library.

In this tutorial, we take a look at the 3-phase software development process that DevMax was designed to support

 1. Design
 2. Test
 3. Implement

DevMax's Design Goal
--------------------

The most important aspect of an IDE is to provide mechanism **to debug the software** such as running automated test cases.

Apart from its storage efficiency, non-bloated nature (you know which I am comparing to) and many other features, the key selling point of DevMax is the __inclusion of a C/C++ interpreter__. In most major C/C++ IDE on the market, you need to compile the source code and link them to get an executable which can then be tested. With an interpreter, there's no need to get the executable: You can run the C++ source code directly and see the output on the IDE's output console.

Such lightweight testing process is DevMax's intended design to support development of background (i.e. non-UI) libraries and programs. Examples are ubiquitious: a web server responding to web requests, a word count program, plain text formatting, calculator, virtual file system, news crawler, image/video processing, file zipping/unzipping, etc. In these scenarios, the developers could design the library API, write the test suites and implement the functionality.

For non-UI software, all we need to do automated testing is the ability to read/write files and output string.

Design
------

All great software starts with a good purpose/intention.

A file path processing library is part of our need to implement a virtual file system in LawTeX. There, we find ourselves in need of a function to _normalize file paths_ to canonical form as `/a/b/../c.txt` and `/a/c.txt` should refer to the same file, to _extract file name_, to _replace file extension by another extension_, etc.

Thus, we bundle these _related_ functions into this library to process the file paths. From our needs, it is easy to come up with the initial API:

```C++
void Normalize(const char *path, char *output, int output_length);
const char *GetFileName(const char *path);
const char *GetFileExtension(const char *path);
void ChangeFileExtension(const char *path, const char *new_extension, char *output, char *length);
```

The resulting API is in the header file [`DMPaths.h`](DMPaths.h).

Test
----

For many people, it would be weird to have test before implementation. Here, we want to illustrate that writing tests and implementing the library can be done concurrently. Once the API and its purpose is known, one developer can write the test cases and the other implements the library.

Let us consider the `Normalize` function. Intuitively, it should returns the simplest equivalent path to the given path and that if two paths identify the same storage object then their normalization should be the same. There are a lot of things to clarify:

 1. Does this function accept only absolute paths, only relative paths or both?
 2. Do we only support Unix paths or also Windows path?
 3. What should happen in case of "invalid" references, for instances in `/../a/` where there's no parent to the root folder?
 4. Do we remove the last path separator in `/a/b/` since `/a/b` technically refers to the same folder albeit we no longer know it is a folder from the string?
 5. Suppose we do account for Windows. Do we convert backslash `\` to forward slash `/` or vice versa?

These are the things that need to be communicated between the 3 people involved: the designer, the tester and the implementer.

Let us say that after 99 corporate meetings, we end up with the decision:

 1. Both absolute and relative paths need to be consider.
 2. Both Unix and Windows need to be supported.
 3. Invalid references `..` stays in the same folder, just think of it as doing a `cd` command: doing a `cd` at root we are still at the root.
 4. Must remove the `/` for consistency. (Otherwise, have to change the meaning of normalization, allowing for normalization to differ by a `/`.)
 5. On Windows, do as a Windows-er do i.e. convert all `/` to `\`.

Now we can write some test cases:

    Input = /a
    Expected = /a

    Input = /a/../b/
    Expected = /b

    Input = C:\Windows/
    Output = C:\Windows\

There are multiple ways to make a test framework for this. The choice in [`Test/TestPathNormalize.cpp`](Test/TestPathNormalize.cpp) is to write a function
```C++
void TestNormalize(const char *path, const char *expected);
```
and then write our main function
```C++
int main(int argc, const char **argv)
{
    TestNormalize("/a", "/a");
    TestNormalize("/a/../b/", "/b");
    TestNormalize("C:\Windows/", "C:\Windows\");
    return 0;
}
```

Implementation
--------------

Now we implement `Normalize`. Note that during the process of implementation, we can easily interpret the [`Test/TestPathNormalize.cpp`](Test/TestPathNormalize.cpp) to see if any test fails.

The first issue the implementer runs into is: How do one distinguish between Unix path and Windows path if the input path is relative? (For absolute path, it is evident.)

After another 99 meetings, the involved parties realize that: Maybe we should always use Unix paths in `Normalize` and add a method
```C++
void GetWindowsPreferredPath(const char *unix_path, char *win_path, int length);
```
to convert Unix path separator `/` to Windows-preferred `\` if needed.

Then we repeat with other functions.