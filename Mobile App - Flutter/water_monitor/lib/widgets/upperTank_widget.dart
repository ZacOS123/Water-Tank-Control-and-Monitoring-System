
import 'package:flutter/material.dart';
import 'package:water_monitor/data/classes.dart';
import 'package:water_monitor/data/functions.dart';


class UpperTankWidget extends StatelessWidget{
  const UpperTankWidget ({super.key});

  @override
  Widget build(BuildContext context){
    return Container(
      width: double.infinity,
      margin: EdgeInsets.only(top: 24, left: 20, right: 20),
      decoration: BoxDecoration(
        color: Theme.of(context).colorScheme.primary,
        border: upperBoarder(),
        borderRadius: BorderRadius.circular(25),
        boxShadow: [
          BoxShadow(
            color: Colors.black.withOpacity(0.3),
            blurRadius: 10,
          ),
        ],
      ),
      child: 
        Column(
          children: [
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: [
                Column(
                  mainAxisAlignment: MainAxisAlignment.start,
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    SizedBox.square(dimension: 10),  //space 
                    Text(' Upper Tank', style: TextStyle(fontFamily: 'Onest',fontSize: 22, fontWeight: FontWeight.w500, color: Colors.grey),),
                    SizedBox.square(dimension: 25),  //space
                    Row(
                      children: [
                        SizedBox.square(dimension: 30),
                        Column(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: [
                            upperPercentage(),  //gets right percentage
                            upperStatus(),
                          ],
                        ),
                      ],
                    ),
                    SizedBox.square(dimension: 30),  //space 
                  ],
                ),
                Container(
                  padding: EdgeInsets.only(top: 25, bottom: 10),
                  child: Stack(
                    alignment: Alignment.bottomCenter,
                    children:[ 
                      Container(
                        height: upperHeight(),  //max 140
                        width: 115, //max 115
                        margin: EdgeInsets.only(bottom: 24),
                        decoration: BoxDecoration(
                          gradient: LinearGradient(
                            colors: [const Color.fromARGB(255, 4, 14, 90), const Color.fromARGB(255, 97, 207, 237)],
                            begin: Alignment.bottomRight,
                            end: Alignment.topLeft,
                          ),
                        borderRadius: BorderRadius.only(bottomLeft: Radius.circular(24), bottomRight: Radius.circular(24)),
                        ),
                      ),
                      Image.asset('assets/images/tank.png', scale: 3.8,),//TANK PICTURE
                      Padding(
                        padding: EdgeInsets.only(bottom: 70),
                        child: Icon(Icons.question_mark, size: 40, color: measurement == null? Colors.red.withOpacity(0.5) : Colors.transparent),
                      )
                    ]
                  ), 
                ),              
              ]
            ),
            remainingTime(),
          ],
        ), 
    );
  }
}