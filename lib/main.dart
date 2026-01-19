import 'package:flutter/material.dart';
import 'audio_engine.dart';

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  final audio = AudioEngine();
  MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(title: Text("Audio Test")),
        body: Column(
          children: [
            ElevatedButton(
              onPressed: () => audio.start("192.168.1.50", 5000),
              child: Text("START"),
            ),
            ElevatedButton(
              onPressed: () => audio.stop(),
              child: Text("STOP"),
            ),
          ],
        ),
      ),
    );
  }
}
