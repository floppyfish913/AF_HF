import QtQuick 2.3

Canvas {
    property var graphA;
    property var graphB;
    property var graphC;

    onPaint: {
        var context = getContext("2d");
        context.fillStyle = "white";
        context.fillRect(0, 0, width, height);

        drawHorizontalLine(context, 0.0, "rgba(0,0,0,1)", 5.0)
        drawHorizontalLine(context, 5.0, "rgba(0,0,0,1)", 5.0)
        drawHorizontalLine(context, -5.0, "rgba(0,0,0,1)", 5.0)
        drawHorizontalLine(context, 10.0, "rgba(0,0,0,1)", 5.0)
        drawHorizontalLine(context, -10.0, "rgba(0,0,0,1)", 5.0)

        drawDataset(context, graphA, "rgba(220,110,110,1)", 5.0);
        drawDataset(context, graphB, "rgba(110,220,110,1)", 5.0);
        drawDataset(context, graphC, "rgba(110,110,220,1)", 5.0);
    }

    function drawHorizontalLine(context, dataValue, strokeStyle, verticalScaler)
    {
        var offset = height/2;
        context.beginPath();
        context.lineWidth = 1;
        context.strokeStyle = strokeStyle;
        context.moveTo(0, offset - verticalScaler * dataValue);
        context.lineTo(width, offset - verticalScaler * dataValue);
        context.stroke();
    }

    function drawDataset(context, datarow, strokeStyle, verticalScaler)
    {
        var offset = height/2;
        context.beginPath();
        context.lineWidth = 3;
        context.strokeStyle = strokeStyle;
        context.moveTo(0, offset-datarow[0]);
        for(var i=0; i<graphA.length;i++)
        {
            context.lineTo(10*i, offset - verticalScaler * datarow[i]);
        }
        context.stroke();
    }
}
