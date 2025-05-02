# Source-CSharp
*Source SDK 2013 With C# Scripting support via .NET.*

## C# Features
- Networked Entity Support
- Interaction with Source's File System
- Cleaned up API
- Input Support
- Bindable Game Events (OnDeath, OnMapChange, etc)



## C# Project Structure
C# Support adds a lot of new folders and files, each with their own conventions. Some of them are as follows:

### C++ Bindings
Stored in `Client OR Server/Source Files/CSharp/Bridge/`. This is where all the C++ Code that gets called by C# is stored. Files made in here should be appended with "Exports" to make their function clear.

### C# Bindings
The C# solution `src/CSharp/Engine/Engine.sln` provides all the abstractions for interacting with Engine code from C#. Inside of the solution the imports are stored in `Bridge/` and often mirror the folder structure of the C++ bindings its importing. Bridge classes should ALWAYS be marked as `internal` to prevent users accessing the low level engine imports.

## Build Instructions

### Prerequisites
 - Source SDK 2013 Multiplayer installed via Steam
 - **Visual Studio 2022** (Windows) or compatible build tools (Linux)
 - **Python 3.13** or later (Windows only)
 - **.NET 9.0** for C# compilation
 - **podman** (Linux only)

### Windows

1. Clone the repository:

    ```bat
    git clone https://github.com/YourUsername/source-cs
    ```

2. Navigate to the `src` folder and run:

    ```bat
    createallprojects.bat
    ```

3. Open `everything.sln` in Visual Studio 2022.

4. Build the solution via `Build > Build Solution`.

5. Copy `nethost.dll` from `src/thirdparty/dotnet` to the `bin/x64` folder of the mod.

6. Build both the `SourceEngine` and optionally `Game` C# Projects in `src/CSharp` and copy the outputs to the `bin/x64` folder of the mod.

7. Set your mod project (e.g., `Client (HL2MP)`) as the startup project.

8. Run with the debugger (`Local Windows Debugger`).

### Linux

Requirements:
 - Source SDK 2013 Multiplayer installed via Steam
 - podman

Inside the cloned directory, navigate to `src`, run:
```bash
./buildallprojects
```

This will build all the projects related to the SDK and your mods automatically against the Steam Runtime.

You can then, in the root of the cloned directory, you can navigate to `game` and run your mod by launching the build launcher for your mod project, eg:
```bash
./mod_tf
```

*Mods that are distributed on Steam MUST be built against the Steam Runtime, which the above steps will automatically do for you.*

## Distributing your Mod

There is guidance on distributing your mod both on and off Steam available at the following link:

https://partner.steamgames.com/doc/sdk/uploading/distributing_source_engine

## Additional Resources

- [Valve Developer Wiki](https://developer.valvesoftware.com/wiki/Source_SDK_2013)

## License

The SDK is licensed to users on a non-commercial basis under the [SOURCE 1 SDK LICENSE](LICENSE), which is contained in the [LICENSE](LICENSE) file in the root of the repository.

For more information, see [Distributing your Mod](#markdown-header-distributing-your-mod).
