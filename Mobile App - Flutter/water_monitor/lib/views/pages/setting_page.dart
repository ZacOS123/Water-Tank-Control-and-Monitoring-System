import 'package:flutter/material.dart';

class SettingPage extends StatelessWidget{
  const SettingPage ({super.key});

  @override
  Widget build(BuildContext context){
    return Center(child:
      Text('Settings', style: TextStyle(
          fontFamily: 'Onest',
          fontSize: 14,
          fontWeight: FontWeight.w800,
          color: const Color.fromARGB(255, 142, 142, 142),
        ),
      )
    );
  }
}