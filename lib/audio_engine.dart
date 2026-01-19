import 'dart:ffi';
import 'package:ffi/ffi.dart';

typedef _StartNative = Void Function(Pointer<Utf8>, Int32);
typedef _StopNative  = Void Function();

class AudioEngine {
  late final DynamicLibrary _lib;
  late final void Function(Pointer<Utf8>, int) _start;
  late final void Function() _stop;

  AudioEngine() {
    _lib = DynamicLibrary.open("audiostream.so");

    _start = _lib
        .lookup<NativeFunction<_StartNative>>("audio_start")
        .asFunction();

    _stop = _lib
        .lookup<NativeFunction<_StopNative>>("audio_stop")
        .asFunction();
  }

  void start(String ip, int port) {
    final ipPtr = ip.toNativeUtf8();
    _start(ipPtr, port);
    malloc.free(ipPtr);
  }

  void stop() {
    _stop();
  }
}

