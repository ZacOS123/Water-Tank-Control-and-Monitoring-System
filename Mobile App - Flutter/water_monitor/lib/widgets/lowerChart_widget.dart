import 'package:flutter/material.dart';
import 'package:fl_chart/fl_chart.dart';

class LowerChartWidget extends StatefulWidget{
  const LowerChartWidget ({super.key});
  @override
  State<LowerChartWidget> createState() => _LowerChartState();
}

class _LowerChartState extends State<LowerChartWidget>{
  int selectedIndex = 0;
  @override
  Widget build(BuildContext context){
    return Container(
      width: double.infinity,
      margin: EdgeInsets.only(top: 24, left: 20, right: 20),
      decoration: BoxDecoration(
        color: Theme.of(context).colorScheme.primary,
        //border: Border.all(),
        borderRadius: BorderRadius.circular(25),
        boxShadow: [
          BoxShadow(
            color: Colors.black.withOpacity(0.3),
            blurRadius: 10,
          ),
        ],
      ),
      child: Column(children: [
        SizedBox.square(dimension: 15),  //space 
        Text(
          'Lower Tank',
          style: TextStyle(
            fontFamily: 'Onest',
            fontSize: 25, 
            fontWeight: FontWeight.w500, 
            color: Colors.grey),
        ),
        SizedBox.square(dimension: 20),  //space 
        Container(
          margin: EdgeInsets.only(right: 35, left: 20, bottom: 28),
          child: SizedBox(
            height: 200,
            child: LineChart(
              LineChartData(
                maxY: 100,
                minY: 0,
                // Lines to draw
                lineBarsData: [
                  // First line
                  LineChartBarData(
                    spots: [
                      FlSpot(0, 10),
                      FlSpot(2, 40),
                      FlSpot(4, 30),
                      FlSpot(6, 70),
                      FlSpot(8, 50),
                      FlSpot(10, 90),
                    ],
                    isCurved: true,
                    barWidth: 4,
                    color: Colors.lightGreenAccent,
                    shadow: Shadow(color: Colors.black.withOpacity(0.2), blurRadius: 10, offset: Offset(0, 5)),
                    isStrokeCapRound: true,
                    dotData: FlDotData(show: false),
                    belowBarData: BarAreaData(
                      show: false,
                    ),
                  ),
                ],
                titlesData: FlTitlesData(
                  topTitles: AxisTitles(
                    sideTitles: SideTitles(showTitles: false), // hide top axis
                  ),
                  rightTitles: AxisTitles(
                    sideTitles: SideTitles(showTitles: false), // hide right axis
                  ),
                  bottomTitles: AxisTitles(
                  sideTitles: SideTitles(
                    showTitles: true,
                    interval: 1,
                    getTitlesWidget: (value, meta) => Text(value.toInt().toString()),
                  ),
                ),
                leftTitles: AxisTitles(
                  sideTitles: SideTitles(
                    showTitles: true,
                    interval: 25,
                    getTitlesWidget: (value, meta) => Text('${value.toInt()}'),
                  ),
                ),
                )
              ),
            ),
          ),
        ),
      ],
      )
    ); 
  }
 }
