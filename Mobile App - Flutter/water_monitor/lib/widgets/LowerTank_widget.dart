import 'package:flutter/material.dart';

class AccumulatorWidget extends StatefulWidget{
  const AccumulatorWidget ({super.key});
  @override
  State<AccumulatorWidget> createState() => _AccumulatorState();
}

class _AccumulatorState extends State<AccumulatorWidget>{
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
      child: 
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: [
            Column(
              mainAxisAlignment: MainAxisAlignment.start,
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                SizedBox.square(dimension: 10),  //space 
                Text(' Lower Tank', style: TextStyle(fontFamily: 'Onest',fontSize: 22, fontWeight: FontWeight.w500, color: Colors.grey),),
                SizedBox.square(dimension: 25),  //space
                Row(
                  children: [
                    SizedBox.square(dimension: 30),
                    Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Text('36%', style: TextStyle(fontFamily: 'Onest',fontSize: 50, fontWeight: FontWeight.w800),),
                        Row(
                          spacing: 7,
                          children: [
                            Container(
                              height: 20,
                              width: 20,
                              decoration: BoxDecoration(
                              color: const Color.fromARGB(255, 50, 200, 65),
                              shape: BoxShape.circle,
                            ),
                            ),
                            Text('Filling', style: TextStyle(fontFamily: 'Onest',fontSize: 20, fontWeight: FontWeight.w700),),
                          ],
                        ),
                      ],
                    ),
                  ],
                ),
                SizedBox.square(dimension: 30),  //space 
              ],
            ),
            Container(
              padding: EdgeInsets.only(top: 25, bottom: 10),
              child: Image.asset('assets/images/tank.png', scale: 3.8,), //TANK PICTURE
            ), 
          ],
        ), 
    );
  }
}