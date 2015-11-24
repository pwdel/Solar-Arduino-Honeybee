% Use thingSpeakRead function to read data for a fixed number of days
% This will abstract away the need to parse and clean up the date data

[data1, time1] = thingSpeakRead(XXXXX, 'Fields', [1], 'NumDays', 15);
[data2, time2] = thingSpeakRead(XXXXX, 'Fields', [2], 'NumDays', 15);

createvoltagecloudfigure(time1,data1,time2,data2)
