#!/bin/sh

DATASTRUCTURES="list hashtable"
NWORDS="1000 5000 10000 25000 50000"

HTML="plot.html"

TITLE="Comparison of list and hashtable operations"
XAXIS="Number of words"
YAXIS="milliseconds"

# --  nothing needs changing below this line  --------------------------

function run_datastructures {
    for d in `echo $DATASTRUCTURES`
    do
	for n in `echo $NWORDS`
	do
	    ./test$d $n
	done		> stats.$d
    done
}

function collate_stats {
    echo
    echo '      data.addRows(['

    cat stats.* | cut -f2 | sort -n -u > col1
    c=1
    for d in `echo $DATASTRUCTURES`
    do
	c=`expr $c + 1`
	cat stats.$d | grep "add"	| cut -f2,3 > col$c
	c=`expr $c + 1`
	cat stats.$d | grep "find"	| cut -f2,3 > col$c
    done

    join col1 col2 | join - col3 | join - col4 | join - col5 | \
	sed -e 's/ms//g' -e 's/ /, /g' -e 's/.*/          [&],/'

    echo '      ]);'
    echo
}

function add_columns {
    echo "      data.addColumn('number', '$XAXIS');"
    for t in `echo $DATASTRUCTURES`
    do
        echo "      data.addColumn('number', '$t-add');"
        echo "      data.addColumn('number', '$t-find');"
    done
}

function build_html {
    cat << END_END
<html>
<head>
  <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
  <script type='text/javascript'>

    google.charts.load('current', {packages: ['corechart', 'line']});
    google.charts.setOnLoadCallback(drawChart);

    function drawChart() {
      var data  = new google.visualization.DataTable();
      var chart = new google.visualization.LineChart(document.getElementById('chart_div'));
      var options = {
        title: '$TITLE',
        hAxis: {
	  title: '$XAXIS'
        },
        vAxis: {
	  title: '$YAXIS',
	  viewWindow: {
	    min: 0
	  }
        },
	curveType: 'function',
        width:  800,
        height: 600
      };
END_END

run_datastructures
add_columns
collate_stats

cat << END_END
      chart.draw(data, options);
    }
  </script>
</head>

<body>
  <div id='chart_div'></div>
</body>
</html>
END_END
}


build_html > $HTML
echo "output is in $HTML"
rm -f col? stats.*
