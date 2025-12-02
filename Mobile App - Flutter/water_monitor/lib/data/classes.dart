import 'dart:async';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'package:flutter/foundation.dart';
import 'constants.dart';


class Docs {
  final List<MeasurementData> documents;
  final int totalDocs;
  final int limit;
  final int totalPages;
  final int page;
  final int pagingCounter;
  final bool hasPrevPage;
  final bool hasNextPage;
  final int? prevPage;
  final int nextPage;

  const Docs({required this.documents,
              required this.totalDocs,
              required this.limit,
              required this.totalPages,
              required this.page,
              required this.pagingCounter,
              required this.hasPrevPage,
              required this.hasNextPage,
              required this.prevPage,
              required this.nextPage,
             }
            );

  factory Docs.fromJson(Map<String, dynamic> json) { //factory is used to apply logic to the data before saving
    return switch (json) {
      {'docs': List docsJson} => Docs(
        documents: docsJson
            .whereType<Map<String, dynamic>>()
            .map((e) => MeasurementData.fromJson(e))
            .toList(),
        totalDocs: json['totalDocs'] as int,
        limit: json['limit'] as int,
        totalPages: json['totalPages'] as int,
        page: json['page'] as int,
        pagingCounter: json['pagingCounter'] as int,
        hasPrevPage: json['hasPrevPage'] as bool,
        hasNextPage: json['hasNextPage'] as bool,
        prevPage: json['prevPage'] as int?,
        nextPage: json['nextPage'] as int,
      ),
      _ => throw const FormatException('Failed to load docs.'),
    };
  }
}

class MeasurementData {
  final List<int> values;
  final String id;
  final DateTime timestamp;
  final String measurement;

  const MeasurementData({
    required this.values,
    required this.id,
    required this.timestamp,
    required this.measurement,
  });

   factory MeasurementData.fromJson(Map<String, dynamic> json) {
    return switch (json) {
      {
        'values': List<dynamic> values,
        '_id': String id,
        'timestamp': String timestamp,
        'measurement': String measurement,
      } =>
        MeasurementData(
          values: values.whereType<int>().toList(),
          id: id,
          timestamp: DateTime.parse(timestamp),
          measurement: measurement,
        ),
      _ => throw const FormatException('Failed to load measurement data.'),
    };
  }
}

class DocsHolder extends ChangeNotifier {
  Docs? _current;
  Timer? _timer;

  Docs? get current => _current;

  DocsHolder() {
    _startAutoRefresh();
  }

  void _startAutoRefresh() {
    fetchDocs();
    _timer = Timer.periodic(Duration(seconds: 2), (_) => fetchDocs());
  }

  Future<void> fetchDocs() async {
    final response = await http.get(
      url, 
      headers: {
        'Authorization': token,
        'Content-Type': 'application/json',
      },
    );
    if (response.statusCode == 200) {
      _current = Docs.fromJson(json.decode(response.body));
      notifyListeners();
    }
    else{
      print('http error code: ${response.statusCode}');
    }
  }

  void dispose() {
    _timer?.cancel();
    super.dispose();
  }
}