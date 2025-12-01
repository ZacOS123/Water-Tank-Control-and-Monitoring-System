import 'package:flutter/material.dart';
import 'package:fl_chart/fl_chart.dart';
import 'package:water_monitor/data/classes.dart';
import 'package:water_monitor/data/functions.dart';

class UpperChartWidget extends StatefulWidget{
  const UpperChartWidget ({super.key});
  @override
  State<UpperChartWidget> createState() => _UpperChartState();
}

class _UpperChartState extends State<UpperChartWidget>{

  @override
  Widget build(BuildContext context){

    if(measurement == null){ //check
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
          'Upper Tank',
          style: TextStyle(
            fontFamily: 'Onest',
            fontSize: 25, 
            fontWeight: FontWeight.w500, 
            color: Colors.grey),
        ),
        Center(child: CircularProgressIndicator(color: Colors.grey, padding: EdgeInsets.all(30))),
      ],
      )
    );
    }

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
          'Upper Tank',
          style: TextStyle(
            fontFamily: 'Onest',
            fontSize: 25, 
            fontWeight: FontWeight.w500, 
            color: Colors.grey),
        ),
        SizedBox.square(dimension: 20),  //space 
        Container(
          padding: EdgeInsets.only(right: 35, left: 12, bottom: 30),
          child: SizedBox(
            height: 255,
            child: LineChart(
              LineChartData(
                maxY: 110,
                minY: 0,
                maxX: 10,
                clipData: FlClipData.all(),
                gridData: const FlGridData(
                  drawVerticalLine: false,   // ⬅️ turns off all grid lines
                ),
                // Lines to draw
                lineBarsData: [
                  // First line
                  LineChartBarData(
                    spots: [
                      FlSpot(9, measurement!.documents[0].values[0].toDouble()),
                      FlSpot(8, measurement!.documents[1].values[0].toDouble()),
                      FlSpot(7, measurement!.documents[2].values[0].toDouble()),
                      FlSpot(6, measurement!.documents[3].values[0].toDouble()),
                      FlSpot(5, measurement!.documents[4].values[0].toDouble()),
                      FlSpot(4, measurement!.documents[5].values[0].toDouble()),
                      FlSpot(3, measurement!.documents[6].values[0].toDouble()),
                      FlSpot(2, measurement!.documents[7].values[0].toDouble()),
                      FlSpot(1, measurement!.documents[8].values[0].toDouble()),
                      FlSpot(0, measurement!.documents[9].values[0].toDouble()),
                    ],
                    preventCurveOverShooting: false,
                    curveSmoothness: 0.2,
                    isCurved: true,
                    barWidth: 4,
                    color: Colors.blueAccent,
                    //gradient: LinearGradient(colors: [Colors.red, Colors.orange, Colors.yellow, Colors.green], begin: Alignment.bottomCenter, end: Alignment.topCenter),
                    shadow: Shadow(color: Colors.black.withOpacity(0.2), blurRadius: 10, offset: Offset(0, 5)),
                    isStrokeCapRound: true,
                    dotData: FlDotData(checkToShowDot: (spot, barData) {
                      return spot.x == 9; // only show dot for spot with x = 9
                    },),
                    belowBarData: BarAreaData(
                      show: true,
                      gradient: LinearGradient(colors: [Colors.red.withOpacity(0.5), Colors.yellow.withOpacity(0.5), Colors.green.withOpacity(0.5)], begin: Alignment.bottomCenter, end: Alignment.topCenter), 
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
                      getTitlesWidget: (value, meta) {
                        switch (value.toInt()) {
                          case 1:
                            return SizedBox(                             
                              width: 50,                             
                              child: Text(upperGraphTime(value.toInt()),
                                softWrap: true, 
                                maxLines: 2, 
                                textAlign: TextAlign.center,
                                style: TextStyle(color: const Color.fromARGB(255, 112, 112, 112),
                                  fontFamily: 'Onest',
                                  fontSize: 12,
                                  fontWeight: FontWeight.w600,
                                ),
                              ),
                            );
                          case 3:
                            return SizedBox(                             
                              width: 50,                             
                              child: Text(upperGraphTime(value.toInt()),
                                softWrap: true, 
                                maxLines: 2, 
                                textAlign: TextAlign.center,
                                style: TextStyle(color: const Color.fromARGB(255, 112, 112, 112),
                                  fontFamily: 'Onest',
                                  fontSize: 12,
                                  fontWeight: FontWeight.w600,
                                ),
                              ),
                            );
                          case 5:
                            return SizedBox(                             
                              width: 50,                             
                              child: Text(upperGraphTime(value.toInt()),
                                softWrap: true, 
                                maxLines: 2, 
                                textAlign: TextAlign.center,
                                style: TextStyle(color: const Color.fromARGB(255, 112, 112, 112),
                                  fontFamily: 'Onest',
                                  fontSize: 12,
                                  fontWeight: FontWeight.w600,
                                ),
                              ),
                            );
                          case 7:
                            return SizedBox(                             
                              width: 50,                             
                              child: Text(upperGraphTime(value.toInt()),
                                softWrap: true, 
                                maxLines: 2, 
                                textAlign: TextAlign.center,
                                style: TextStyle(color: const Color.fromARGB(255, 112, 112, 112),
                                  fontFamily: 'Onest',
                                  fontSize: 12,
                                  fontWeight: FontWeight.w600,
                                ),
                              ),
                            );
                          case 9:
                            return SizedBox(                             
                              width: 50,                             
                              child: Text(upperGraphTime(value.toInt()),
                                softWrap: true, 
                                maxLines: 2, 
                                textAlign: TextAlign.center,
                                style: TextStyle(color: const Color.fromARGB(255, 112, 112, 112),
                                  fontFamily: 'Onest',
                                  fontSize: 12,
                                  fontWeight: FontWeight.w600,
                                ),
                              ),
                            );
                        }
                        return Container();
                      }
                    ),
                  ),
                  leftTitles: AxisTitles(
                    sideTitles: SideTitles(
                      showTitles: true,
                      reservedSize: 35,
                      interval: 25,
                      getTitlesWidget: (value, meta) { 
                        if (value == meta.max) {
                          return const SizedBox.shrink(); // hide it
                        }
                        return Text(
                                '${value.toInt()}', 
                                style: TextStyle(color: const Color.fromARGB(255, 112, 112, 112),
                                  fontFamily: 'Onest',
                                  fontSize: 12,
                                  fontWeight: FontWeight.w600,
                                ),
                                textAlign: TextAlign.center,
                                );
                      }
                  ),
                ),
                )
              ),
            ),
          ),
        ),
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: [
            upperStatus(),
            Column(
              mainAxisAlignment: MainAxisAlignment.start,
              children: [
                Text('Last Update:',
                  style: TextStyle(fontFamily: 'Onest', fontSize: 12, fontWeight: FontWeight.w400, color: const Color.fromARGB(255, 170, 170, 170))
                ),
                Text(upperGraphTime(0),
                  style: TextStyle(fontFamily: 'Onest', fontSize: 16, fontWeight: FontWeight.w600, color: const Color.fromARGB(255, 170, 170, 170))
                ),
              ],
            )
          ],
        ),
        SizedBox(height: 20,)
      ],
      )
    );
  }
}
