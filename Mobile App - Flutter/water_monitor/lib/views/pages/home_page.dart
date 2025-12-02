import 'package:flutter/material.dart';
import 'package:water_monitor/widgets/LowerTank_widget.dart';
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
    //getData();
  }
/*
  Future<void> firstRefresh() async{
    await getData();
    setState(() {});
  }
*/

  @override
  Widget build(BuildContext context){
    return /*RefreshIndicator(
      color: Theme.of(context).colorScheme.onPrimary,
      backgroundColor: Theme.of(context).colorScheme.primary,
      onRefresh: () async{
        await getData();
        setState(() {});
      },
      child:*/
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
          AccumulatorWidget(),
          SizedBox(height: 30),
        ]
        ),]
      );
    //);
  }
}