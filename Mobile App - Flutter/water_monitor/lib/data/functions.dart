
import 'package:flutter/material.dart';
import 'package:water_monitor/data/classes.dart';
import 'package:provider/provider.dart';

/*
Future getData() async {

    // Await the http get response, then decode the json-formatted response.
    var response = await http.get(
      url, 
      headers: {
        'Authorization': '$token',
        'Content-Type': 'application/json',
      },
    );
    
    if (response.statusCode == 200) {
      Map<String, dynamic> jsonData = jsonDecode(response.body);
      measurement = Docs.fromJson(jsonData);
      print(measurement!.documents[0].values[0]);
      return measurement;
    }
    else {
      print('Request failed with status: ${response.statusCode}.');
      return null;
    }
  }*/



  

Widget upperPercentage (BuildContext context){
  final measurement = context.watch<DocsHolder>().current;
  Widget returnedWidget = Text(
              '--', 
              style: TextStyle(
                fontFamily: 'Onest',
                fontSize: 50,
                fontWeight: FontWeight.w800),
              );

  if(measurement == null){
    return returnedWidget;
  }
  else{
    for(int i=0; i<9; i++){
      if((measurement.documents[i].values[1] & 0x01) == 0){
        returnedWidget = Text(
                    '${measurement.documents[i].values[0]}%',
                    key: ValueKey(measurement.documents[i].values[0]),
                    style: TextStyle(
                        fontFamily: 'Onest',
                        fontSize: 50,
                        fontWeight: FontWeight.w800),
                    ); 
        break; 
      }
    }
  }
  return returnedWidget;
}

////////////////////////////////////////

Widget upperStatus(BuildContext context){

  final measurement = context.watch<DocsHolder>().current;


  if(measurement == null){
    return SizedBox(height: 2.0);
  }
  else if(measurement.documents[0].values[1] & 0x08 == 8){ //if pumping
    return Row(
            spacing: 7,
            children: [
              Icon(Icons.pending, color: Color.fromARGB(255, 58, 110, 255), size: 26),
              Text('Filling', style: TextStyle(fontFamily: 'Onest',fontSize: 20, fontWeight: FontWeight.w700),),
            ],
          );
  }
  else if(measurement.documents[0].values[1] & 0x01 == 1){ //sensor error
    return Column(
      children: [
        Row(
            spacing: 7,
            children: [
              Icon(Icons.error, color: Colors.red, size: 20),
              Text('Sensor Error', style: TextStyle(fontFamily: 'Onest', fontSize: 15, fontWeight: FontWeight.w500, color: Colors.red),),
            ],
          ),
        SizedBox(height: 10),
        Container(
          width: 125,
          child: Text('Showing last valid water level.', style: TextStyle(fontFamily: 'Onest', fontSize: 12, fontWeight: FontWeight.w400, color: const Color.fromARGB(255, 131, 131, 131)), softWrap: true,)),
      ],
    );
  }
  else if(measurement.documents[0].values[1] & 0x02 == 2){  //pump error
    return Row(
              spacing: 7,
              children: [
                Icon(Icons.error, color: Colors.red, size: 26),
                SizedBox(
                  width: 125,
                  child: Text('Pump Malfunctioning', 
                    style: TextStyle(fontFamily: 'Onest',fontSize: 14, fontWeight: FontWeight.w500, color: Colors.red),
                    softWrap: true
                  ),
                ),
              ],
            );
  }
  else if(measurement.documents[0].values[1] & 0x04 == 4){  //pump error
    return Row(
              spacing: 7,
              children: [
                Icon(Icons.bluetooth_disabled, color: Colors.red, size: 23),
                Text('Can\'t Fill', style: TextStyle(fontFamily: 'Onest',fontSize: 15, fontWeight: FontWeight.w500, color: Colors.red),),
              ],
            );
  }
  else if(measurement.documents[0].values[0] == 100){
    return Row(
              spacing: 7,
              children: [
                Icon(Icons.check_circle, color: Color.fromARGB(255, 74, 209, 1), size: 25),
                Text('Full', style: TextStyle(fontFamily: 'Onest',fontSize: 20, fontWeight: FontWeight.w700),),
              ],
            );
  }
  return Row(
            spacing: 7,
            children: [
              Icon(Icons.pause_circle_filled, color: const Color.fromARGB(255, 228, 201, 0), size: 25),
              Text('Waiting', style: TextStyle(fontFamily: 'Onest',fontSize: 20, fontWeight: FontWeight.w600), softWrap: true,),
            ],
          );
}

////////////////////////////

Border upperBoarder (BuildContext context){

  final measurement = context.watch<DocsHolder>().current;

  if(measurement != null){
    if(measurement.documents[0].values[1] & 0x02 == 2 ||
     measurement.documents[0].values[1] & 0x01 == 1 ||
     measurement.documents[0].values[1] & 0x04 == 4){

    return Border.all(color: Colors.red.withOpacity(0.5), width: 5.0);  
    }
  }
  return Border.all(color: Colors.transparent);
}

////////////////

Widget upperHeight (BuildContext context){

  final measurement = context.watch<DocsHolder>().current;

  if(measurement != null){
    if(measurement.documents[0].values[1] & 0x01 != 1){
      return Container(
      height: (measurement.documents[0].values[0] * 140) / 100,  //max 140
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
    );
    }
  }
  return Padding(
    padding: EdgeInsets.only(bottom: 70),
    child: Icon(Icons.priority_high_rounded, size: 40, color: Colors.red.withOpacity(0.5)),
  );
}

//////////////

String upperGraphTime(BuildContext context, int n) {

  final measurement = context.watch<DocsHolder>().current;

  if(measurement != null){
    DateTime dt = measurement.documents[9-n].timestamp;
    
    return '${dt.hour.toString().padLeft(2, '0')}:${dt.minute.toString().padLeft(2, '0')}  ${dt.day.toString().padLeft(2, '0')}/${dt.month.toString().padLeft(2, '0')}';
  }
  else{
    return '--';
  }
}

Widget remainingTime(BuildContext context){

  final measurement = context.watch<DocsHolder>().current;

  if(measurement != null){
    int time = (measurement.documents[0].values[1] & 0x0000FF00) >> 8;
    if(time > 0  && measurement.documents[0].values[1] & 0x08 == 8){
      return Container(
        padding: EdgeInsets.only(top: 6, bottom: 10),
        child: Center(
          child:  Text(
            'About $time minutes until full.',
            style: TextStyle(
              fontFamily: 'Onest',
              fontSize: 12,
              fontWeight: FontWeight.w400,
              color: const Color.fromARGB(255, 169, 169, 169)
            ),  
          ) 
        ),
      );
    }
  }
  return SizedBox(width: 0);
}