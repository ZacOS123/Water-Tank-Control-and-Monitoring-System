
// ignore_for_file: file_names

import 'package:flutter/material.dart';
import 'package:water_monitor/data/classes.dart';
import 'package:water_monitor/data/functions.dart';
import 'package:provider/provider.dart';

class UpperTankWidget extends StatelessWidget{
  const UpperTankWidget ({super.key});

  @override
  Widget build(BuildContext context){
    final measurement = context.watch<DocsHolder_A>().current;

      return Container(
          width: double.infinity,
          margin: EdgeInsets.only(top: 24, left: 20, right: 20),
          decoration: BoxDecoration(
            color: Theme.of(context).colorScheme.primary,
            border: border(context, measurement),
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
                                percentage(context, measurement),  //gets right percentage
                                upperStatus(context, measurement),
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
                          height(context, measurement),
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
                remainingTime(context, measurement),
              ],
            ), 
          );
        }
}
   