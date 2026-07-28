# Getting Started

## Download And Build

```
> mkdir ltd_home
> cd ltd_home
> mkdir projects
> cd projects
> git clone https://github.com/benniadham/ltd.git
> cd ltd
> LTD_HOME=/path/to/your/ltd_home; export LTD_HOME
> source ./bootstrap.sh
```

## Start A New Project

1. Go to `projects` directory.
2. Create a new directory for the new project
```
> mkdir helloworld
> cd helloworld
```
3. Create a new folder called `app`
```
helloworld > mkdir app
helloworld > cd app
app > _
```
4. Add your header and source file for your app binary here. The binary name will be the same as the project directory name.
5. Once you finished editing your source file you can start the build as follow:
    1. first set the active project 
    ```
    app > ltd cd helloworld
    ```
    2. build the project
    ``` 
    app > ltd build
    ```
    3. Or you can specify the target
    ```
    app > ltd build --target=app
    ```
    4. Use `-v` flags to increate verbosity. i.e.
    ```
    app > ltd build --target=app -vvv
    ```
6. If you need to create library in this project you can add `lib` and `inc` directory to the project directory. Put your source files in the `lib` directory and your header files in `inc/you_project_name` directory. Your library will be named `libyour_library_name.a`.
```
app > cd ..
helloworld > mkdir lib
helloworld > mkdir inc
helloworld > cd inc
helloworld > mkdir helloworld
```
7. If you need to make more than one library, you can create them under `libs` directory instead. i.e.:
```
.
└── helloworld
    ├── apps
    │   ├── application_1
    │   └── application_2
    ├── inc
    │   ├── lib1
    │   └── lib2
    └── libs
        ├── lib1
        └── lib2
```
    In this example, your binaries will be named `app1` and `app2`. Your library will be named `lib1` and `lib2`.