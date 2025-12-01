// ignore_for_file: camel_case_types

import 'package:flutter/material.dart';
import 'package:water_monitor/data/notifiers.dart';

class NavBar extends StatelessWidget{
  const NavBar ({super.key});
  
  @override
  Widget build(BuildContext context){
    return ValueListenableBuilder(
      valueListenable: selectedPageNotifier,
      builder:(context, selectedPage, child) {
        return NavigationBar(
          indicatorColor: const Color.fromARGB(255, 192, 192, 192),
          elevation: 5.0,
          shadowColor: Colors.black.withOpacity(0.3),
          backgroundColor: Theme.of(context).colorScheme.primary,
          destinations: [
            NavigationDestination(icon: Icon(Icons.home), label: 'home'),
            NavigationDestination(icon: Icon(Icons.history_rounded), label: 'history'),
            NavigationDestination(icon: Icon(Icons.settings), label: 'settings'),
          ],
          onDestinationSelected: (int value) {
            selectedPageNotifier.value = value;
          },
          selectedIndex: selectedPage,

        );
      },
    );
  }
}