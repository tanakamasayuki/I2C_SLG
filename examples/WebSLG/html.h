const char* htmlBodyString = R"html(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>#TITLE#</title>
</head>
<body>
#BODY#
</body>
</html>
)html";

const char* htmlIODisplayString = R"html(
<H2>Control Code = #CONTROL_CODE#</H2>
<H3>GPIO</H3>
<table border="1">
<tr><th>IO</th><th>Level</th><th>VCC</th><th>Description</th></tr>
<tr><td>0</td><td>#IO0#</td><td>VCC</td><td>GPIO</td></tr>
<tr><td>1</td><td>#IO1#</td><td>VCC</td><td>GPIO/Vref IN</td></tr>
<tr><td>2</td><td>#IO2#</td><td>VCC</td><td>GPIO/SLA_0</td></tr>
<tr><td>3</td><td>#IO3#</td><td>VCC</td><td>GPIO/SLA_1</td></tr>
<tr><td>4</td><td>#IO4#</td><td>VCC</td><td>GPIO/SLA_2</td></tr>
<tr><td>5</td><td>#IO5#</td><td>VCC</td><td>GPIO/SLA_3</td></tr>
<tr><td>6</td><td></td><td>VCC</td><td>GPO(Output ONLY)</td></tr>
<tr><td>7</td><td></td><td>VCC2</td><td>GPO(Output ONLY)</td></tr>
<tr><td>8</td><td>#IO8#</td><td>VCC2</td><td>GPIO</td></tr>
<tr><td>9</td><td>#IO9#</td><td>VCC2</td><td>GPIO/Vref_OUT1</td></tr>
<tr><td>10</td><td>#IO10#</td><td>VCC2</td><td>GPIO/Vref_OUT0</td></tr>
<tr><td>11</td><td>#IO11#</td><td>VCC2</td><td>GPIO/ACMP3L_IN</td></tr>
<tr><td>12</td><td>#IO12#</td><td>VCC2</td><td>GPIO/ACMP2L_IN</td></tr>
<tr><td>13</td><td>#IO13#</td><td>VCC2</td><td>GPIO/ACMP1H_IN</td></tr>
<tr><td>14</td><td>#IO14#</td><td>VCC2</td><td>GPIO/ACMP0H_IN</td></tr>
</table>
)html";

const char* htmlWriteString = R"html(
<h3>Write</h3>
<form action="write" method="get">
  <input type="hidden" name="controlCode" value="#CONTROL_CODE#" />
  <textarea name="data" rows="4" cols="60">

</textarea><br />
  <input type="submit">
</form>
)html";
