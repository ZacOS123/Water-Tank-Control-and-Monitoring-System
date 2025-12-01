import 'package:flutter/material.dart';
import 'package:water_monitor/data/notifiers.dart';
import 'package:water_monitor/views/widget_tree.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget{
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {

  @override
  Widget build(BuildContext context) {
    return ValueListenableBuilder(
      valueListenable: isDarkModeNotifier,
      builder: (context, isDarkMode, child) {
        return MaterialApp(
          theme: ThemeData(
            colorScheme: ColorScheme.light(
              primary: Colors.white,
              onPrimary: Colors.black,
              secondary: Colors.grey,
              brightness: Brightness.light,
              ),
          ),
          darkTheme: ThemeData(colorScheme: ColorScheme.dark(
              primary: const Color.fromARGB(255, 40, 40, 40),
              onPrimary: Colors.white,
              secondary: Colors.grey,
            )
          ),
          themeMode: isDarkMode? ThemeMode.dark : ThemeMode.light,
          home: widgetTree()
        );
      }
    );
  }
}
