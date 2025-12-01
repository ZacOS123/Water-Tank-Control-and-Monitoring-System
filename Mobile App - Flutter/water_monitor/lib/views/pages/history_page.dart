

import 'package:flutter/material.dart';
import 'package:water_monitor/widgets/lowerChart_widget.dart';
import 'package:water_monitor/widgets/upperChart_widget.dart';
import 'package:water_monitor/data/functions.dart';


class HistoryPage extends StatefulWidget{
  const HistoryPage ({super.key});

 @override
  State<HistoryPage> createState() => _HistoryPage();
}

class _HistoryPage extends State<HistoryPage>{

  @override
  void initState() { 
    super.initState();
  }

  Future<void> firstRefresh() async{
    await getData();
    setState(() {});
  }

  @override
  Widget build(BuildContext context){
    return RefreshIndicator.adaptive(
      color: Theme.of(context).colorScheme.onPrimary,
      backgroundColor: Theme.of(context).colorScheme.primary,
      onRefresh: () async{
        await getData();
        setState(() {});
      },
      child: ListView(
        physics: AlwaysScrollableScrollPhysics(),
        children:[ Column(children: [
          Text('History', style: TextStyle(
              fontFamily: 'Onest',
              fontSize: 30,
              fontWeight: FontWeight.w600,
              color: const Color.fromARGB(255, 192, 192, 192),
            ),
          ),
          UpperChartWidget(),
          LowerChartWidget(),
          SizedBox(height: 30),
        ]
        ),]
      ),
    );
  }
}