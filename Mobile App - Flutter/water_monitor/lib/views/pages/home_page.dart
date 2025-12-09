import 'package:flutter/material.dart';
import 'package:water_monitor/widgets/lowerTank_widget.dart';
import 'package:water_monitor/widgets/upperTank_widget.dart';


class HomePage extends StatefulWidget{
  const HomePage ({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage>{
  
  @override
  void initState() { 
    super.initState();
  }

  @override
  Widget build(BuildContext context){
    return 
       ListView(
        physics: AlwaysScrollableScrollPhysics(),
        children: [Column(children: [
          Text('Home', style: TextStyle(
              fontFamily: 'Onest',
              fontSize: 30,
              fontWeight: FontWeight.w600,
              color: const Color.fromARGB(255, 192, 192, 192),
            ),
          ),
          UpperTankWidget(),
          LowerTankWidget(),
          SizedBox(height: 30),
        ]
        ),]
      );
  }
}