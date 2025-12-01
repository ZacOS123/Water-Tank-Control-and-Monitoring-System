//Contains:
//ValueNotifiers: saves data (index for pages)
//ValueListenableBuilder: listens to changes in data

import 'package:flutter/material.dart';

ValueNotifier<int>  selectedPageNotifier = ValueNotifier(0);
ValueNotifier<bool>  isDarkModeNotifier = ValueNotifier(false);

