%% -- FUNCTION --
%% Find SunUp / Sunset Data
% [data3,time3] ...

SunUpHours = hours(7);
SunDownHours = hours(16);

%% -- FUNCTION --
%% Find Earliest Time Window

EarliestTimePoint = time1(1);

%% -- FUNCTION --
%% Find Latest Time Window

[length,n] = size(time1);
LatestTimePoint = time1(length);

%% -- FUNCTION --
%% Find Begining of Day For First Day
%% Start and Stop of First Day of First Datapoint

AnchorFirstDayTime = dateshift(EarliestTimePoint,'start','day');
% AnchorFirstDayTime

%% -- FUNCTION --
%% Find Begining of Day For Second Day
%% Start and Stop of First Day of First Datapoint

StartSecondDay = AnchorFirstDayTime+days(1);
StopSecondDay= AnchorFirstDayTime+days(1);

%% -- FUNCTION --
%% Find Starting Time & Ending Time for First Solar Day & Night

if (AnchorFirstDayTime+SunUpHours) >= EarliestTimePoint
    RealFirstSolarDayStart = AnchorFirstDayTime+SunUpHours;
    RealFirstSolarDayStop = AnchorFirstDayTime+SunDownHours;
    RealFirstSolarNightStart = RealFirstSolarDayStop;
    RealFirstSolarNightStop = RealFirstSolarDayStart+days(1);
else if (AnchorFirstDayTime+SunUpHours < EarliestTimePoint) && (EarliestTimePoint < AnchorFirstDayTime+SunUpHours)
    RealFirstSolarDayStart = StartSecondDay+SunUpHours;
    RealFirstSolarDayStop = StopSecondDay+SunDownHours;
    RealFirstSolarNightStart = AnchorFirstDayTime+SunDownHours;
    RealFirstSolarNightStop = RealFirstSolarDayStart;
    else    
    RealFirstSolarDayStart = StartSecondDay+SunUpHours;
    RealFirstSolarDayStop = StopSecondDay+SunDownHours;
    RealFirstSolarNightStart = StartSecondDay+SunDownHours;
    RealFirstSolarNightStop = StartSecondDay+days(1)+SunUpHours; 
    end
end

%% -- FUNCTION --
%% Find End of Day For Last Day
%% Start and Stop of Last Day of Last Datapoint

StartLastDay = dateshift(LatestTimePoint,'start','day');
StopLastDay = dateshift(LatestTimePoint,'end','day');

%% -- FUNCTION --
%% Find Begining of Day For Second to Last Day

StartSecondToLastDay = StartLastDay-days(1);
StopSecondToLastDay= StopLastDay-days(1);

%% -- FUNCTION --
%% Find Begining of Day For Third to Last Day

StartThirdToLastDay = StartLastDay-days(2);
StopThirdToLastDay= StopLastDay-days(2);

%% -- FUNCTION --
%% Find Starting Time & Ending Time for Last Solar Day

if (StartLastDay < LatestTimePoint) && (LatestTimePoint <= StartLastDay+SunUpHours)
    RealLastSolarDayStart = StartSecondToLastDay+SunUpHours;
    RealLastSolarDayStop = StartSecondToLastDay+SunDownHours;
    RealLastSolarNightStart = StartThirdToLastDay+SunDownHours;
    RealLastSolarNightStop = StartSecondToLastDay+SunUpHours;
else if (StartLastDay+SunUpHours > LatestTimePoint) && (LatestTimePoint < StartLastDay+SunDownHours)
    RealLastSolarDayStart = StartSecondToLastDay+SunUpHours;
    RealLastSolarDayStop = StartSecondToLastDay+SunDownHours;
    RealLastSolarNightStart = StartSecondToLastDay+SunDownHours;
    RealLastSolarNightStop = StartLastDay+SunUpHours;
    else    
    RealLastSolarDayStart = StartLastDay+SunUpHours;
    RealLastSolarDayStop = StartLastDay+SunDownHours;
    RealLastSolarNightStart = StartSecondToLastDay+SunDownHours;
    RealLastSolarNightStop = StartLastDay+SunUpHours;
    end
end

%% -- FUNCTION --
%% Find Total Duration Of All DataPoints
TotalEndPointsDuration = LatestTimePoint - EarliestTimePoint;

% TotalWindowDuration = 


%% -- FUNCTION --
%% Find Total Solar Days

% RealFirstSolarDayStart
RealFirstSolarDayStart;
% RealLastSolarDayStart
RealLastSolarDayStop;

% Difference Array
NumSolarDaysHoursDuration = RealLastSolarDayStop-RealFirstSolarDayStart;
NumSolarDaysHours = hours(NumSolarDaysHoursDuration);

ExtraSunlightDayDifference = (NumSolarDaysHours/24)-floor(NumSolarDaysHours/24);
MinimumSunlightHours = 0.3674;

if ExtraSunlightDayDifference >= MinimumSunlightHours
    NumSolarDaysInt=floor(NumSolarDaysHours/24);
end

%% --FUNCTION --
%% Find Array of Solar Days
SolarDayStartTimesArray = RealFirstSolarDayStart+days([0:NumSolarDaysInt]);
SolarDayStopTimesArray = RealFirstSolarDayStop+days([0:NumSolarDaysInt]);
% SolarNightStartTimesArray

%% -- FUNCTION --
%% Create Solar Day Start and Stop Index Array

[n,StartTimesArray_Length] = size(SolarDayStartTimesArray);

% Put in Numerical Long Format, Transpose SolarDayStartTimes to fit Format
format long
SolarDayStartTimesArray = transpose(datenum(SolarDayStartTimesArray));
SolarDayStopTimesArray =  transpose(datenum(SolarDayStopTimesArray));
time1_numerical=datenum(time1);

% Search for closest point in original array

RealVoltStartTimeIndex = dsearchn(time1_numerical,SolarDayStartTimesArray);
RealVoltStopTimeIndex = dsearchn(time1_numerical,SolarDayStopTimesArray);

%% -- FUNCTION --
%% Using Start and Stop Index Array, Find Times

FinalRealStartTimes = time1([RealVoltStartTimeIndex]);
FinalRealStopTimes = time1([RealVoltStopTimeIndex]);


%% -- FUNCTION --
%% Using Start and Stop Index Array, Find Voltages

format short

FinalRealStartVolts = data1([RealVoltStartTimeIndex]);
FinalRealStopVolts = data1([RealVoltStopTimeIndex]);


%% -- FUNCTION --
%% Plot Everything
[m,n]=size(FinalRealStartVolts);

%% -- FUNCTION --
%% Find Voltage Deltas & Plot
[m,n]=size(FinalRealStartVolts);

for i=1:m
    VoltageDelta(i)=FinalRealStopVolts(i)-FinalRealStartVolts(i);
end

%% -- FUNCTION --
%% Cloud Coverage Closest End Points

format long
time2_numerical = datenum(time2);

RealCloudStartTimeIndex = dsearchn(time2_numerical,SolarDayStartTimesArray);
RealCloudStopTimeIndex = dsearchn(time2_numerical,SolarDayStopTimesArray);


%% -- FUNCTION --
%% Cloud Days Actual Times Array

RealCloudStartTimeActuals = time2([RealCloudStartTimeIndex]);
RealCloudStopTimeActuals = time2([RealCloudStopTimeIndex]);

%% -- FUNCTION --
%% Cloud Days Average Percent Cloudcover

[m,n]=size(RealCloudStartTimeActuals);
[p,q]=size(RealCloudStopTimeActuals);

% start first day
% from RealCloudStartTimeIndex(i) to RealCloudStopTimeIndex(i)
% put times into day1 (row 1)
% then go to next day
RealCloudFirstIndex=RealCloudStartTimeIndex(1):RealCloudStopTimeIndex(1);
[r,s]=size(RealCloudFirstIndex);
CloudDayIndexArray=zeros(1,s);
for i=1:p
    % Counting Across The RealCloudStartTimeActuals [Each Solar Day]
    [r,s]=size([RealCloudStartTimeIndex(i):RealCloudStopTimeIndex(i)]);
    if s == 6
        CloudDayIndexArray = [CloudDayIndexArray;RealCloudStartTimeIndex(i):RealCloudStopTimeIndex(i)];
    % Account for Smaller Size Rows (Data Point Skipped) - Add an i+1
    % We are assuming the cloudiness of +2 hours was similar to the
    % cloudiness of -2 hours
    elseif s == 5
        CloudDayIndexArray = [CloudDayIndexArray;RealCloudStartTimeIndex(i):(RealCloudStopTimeIndex(i)+1)];
    elseif s == 4
        CloudDayIndexArray = [CloudDayIndexArray;RealCloudStartTimeIndex(i):(RealCloudStopTimeIndex(i)+2)];
    elseif s == 3
        CloudDayIndexArray = [CloudDayIndexArray;RealCloudStartTimeIndex(i):(RealCloudStopTimeIndex(i)+3)];
    elseif s == 4
        CloudDayIndexArray = [CloudDayIndexArray;RealCloudStartTimeIndex(i):(RealCloudStopTimeIndex(i)+4)];
    end
end

% Cut off first row of zeros

CloudDayIndexArray=CloudDayIndexArray(2:p,:);
CloudDayIndexArray = [CloudDayIndexArray;RealCloudStartTimeIndex(p):(RealCloudStopTimeIndex(p))];

%% -- FUNCTION --
%% Average Cloud Function

CloudMeasurementsMatrix = data2(CloudDayIndexArray);
CloudMeasurementsAvgArray = mean(CloudMeasurementsMatrix.');


%% -- FUNCTION --
%% Plot Cloudiness vs. Voltage Change

% http://affect.media.mit.edu/projectpages/affective_cognitive_decision_making/DM%20Experiment/opt_reg_tips/html/optimtips.html#2


% Regression Coefficient
% coef=transpose(CloudMeasurementsAvgArray)\transpose(VoltageDelta);
% yCalc1 = b1*(CloudMeasurementsAvgArray);

[row,col]=size(transpose(CloudMeasurementsAvgArray))

% lets fit this data with our same linear model.
M2 = [ones((row),1),transpose(CloudMeasurementsAvgArray)];
coef = M2\transpose(VoltageDelta)
% and plot the results

yhat2 = M2*coef;
% plot(transpose(CloudMeasurementsAvgArray),transpose(VoltageDelta),'o',transpose(CloudMeasurementsAvgArray),yhat2,'-')
title 'Linear model through quadratic data'

% Correlation Coefficient of Cloud Cover & Voltage Delta
R = corrcoef(CloudMeasurementsAvgArray, VoltageDelta)
Cov = cov(CloudMeasurementsAvgArray, VoltageDelta);


InterceptChart = num2str(yhat2(1),3);
SlopeChart = num2str(coef(2),3);
Equation = strcat('y = ',SlopeChart,'x + ',InterceptChart);

scatter(CloudMeasurementsAvgArray, VoltageDelta)
hold on
plot(transpose(CloudMeasurementsAvgArray),yhat2,'-')
xlim=get(gca,'XLim');
ylim=get(gca,'YLim');
text(0.8*xlim(1)+0.2*xlim(2),0.3*ylim(1)+0.7*ylim(2),Equation)
xlabel('Cloud Cover Percent Average')
ylabel('Volt Delta Per Day')
title('Volt Delta vs. Cloud Cover Average')
grid on
legend('Data','Slope','Location','best');
