import 'package:flutter/material.dart';
import 'package:water_monitor/data/constants.dart';
import 'package:water_monitor/data/notifiers.dart';
import 'package:water_monitor/views/pages/history_page.dart';
import 'package:water_monitor/views/pages/home_page.dart';
import 'package:water_monitor/views/pages/setting_page.dart';
import 'package:water_monitor/widgets/navbar_widget.dart';
import 'package:shared_preferences/shared_preferences.dart';

List<Widget> pages= [
  HomePage(),
  HistoryPage(),
  SettingPage(),
];

class widgetTree extends StatefulWidget{
  const widgetTree ({super.key});

 @override
  State<widgetTree> createState() => _widgetTreeState();
}

class _widgetTreeState extends State<widgetTree>{

  
  @override
  Widget build (BuildContext context){
    return Scaffold(
        backgroundColor: Theme.of(context).colorScheme.primary,
        appBar: AppBar(
          backgroundColor: Theme.of(context).colorScheme.primary,
          actions: [
            IconButton(
              onPressed: () async {
                isDarkModeNotifier.value = !isDarkModeNotifier.value;
                final SharedPreferences prefs = await SharedPreferences.getInstance();
                await prefs.setBool(KConstants.themeModeKey, isDarkModeNotifier.value);
              },
              icon: ValueListenableBuilder(
                valueListenable: isDarkModeNotifier,
                builder: (context, isDarkMode, child) {
                  return Icon(
                    isDarkMode? Icons.light_mode : Icons.dark_mode,
                  );
                }
              ))
          ],
        ),
        bottomNavigationBar: NavBar(),
        body: ValueListenableBuilder(
                valueListenable: selectedPageNotifier,
                builder: (context, selectedPage, child){
                  return pages.elementAt(selectedPage);
                }
              ),
      );
  }
}