import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "/home/natxo/Documentos/of_v0.9.8_linux64_release/libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: '/home/natxo/Documentos/of_v0.9.8_linux64_release'

    ofApp {
        name: { return FileInfo.baseName(path) }

        files: [
            "src/main.cpp",
            "src/ofApp.cpp",
            "src/ofApp.h",
            "src/processTweet.h",
            "src/processTweet.cpp",
            "src/bdCommands.h",
            "src/ofxImageSequenceRecorder.h",
        ]

        // This project is using addons.make to include the addons
        // since it was imported from old code. To change it to include
        // the addons from the qbs file change the following lines to
        // the list of used addons in array format. eg:
        //
         of.addons: [
             'ofxMySQL',
             'ofxAsyncTextureLoader',
         ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include
        of.includePaths: ['/usr/include/cups/']     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: ['/home/natxo/Documentos/of_v0.9.8_linux64_release/addons/ofxMySQL/libs/libmysql/lib/linux64/libmysqlclient.a','/usr/lib/x86_64-linux-gnu/libcups.so']      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
        // and can be checked with #ifdef or #if in the code
        of.frameworks: []       // osx only, additional frameworks to link with the project

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }
    }

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
