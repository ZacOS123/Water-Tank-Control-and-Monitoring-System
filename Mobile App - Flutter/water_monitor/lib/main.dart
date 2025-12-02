
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:water_monitor/data/notifiers.dart';
import 'package:water_monitor/views/widget_tree.dart';
import 'package:water_monitor/data/classes.dart';


void main() {
  runApp(ChangeNotifierProvider(
      create: (_) => DocsHolder(),   // starts auto-refresh inside constructor
      child: const MyApp(),
    ),);
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
