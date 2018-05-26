#include "plot.h"

static FILE *fi;

void plot_init(const char *file_path)
{
	fi = fopen(file_path, "w");
	fprintf(fi, "<!DOCTYPE html>\n");
	fprintf(fi, "<meta charset=\"utf-8\">\n");
	fprintf(fi, "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\" type=\"text/css\" />\n");
	fprintf(fi, "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>\n");
	fprintf(fi, "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/echarts/4.1.0/echarts.min.js\"></script>\n");
	fprintf(fi, "\n");
	fprintf(fi, "<body>\n");
	fprintf(fi, "<div id=\"cover\" style=\"width: 1000px; height: 500px; margin: auto\"></div>\n");
	fprintf(fi, "<div id=\"mlc_len\" style=\"width: 1000px; height: 500px; margin: auto\"></div>\n");
	fprintf(fi, "<div id=\"mlc_cover\" style=\"width: 1000px; height: 500px; margin: auto\"></div>\n");
	fprintf(fi, "\n");
	fprintf(fi, "<script>\n");
}

void plot_coverage(const int64_t *cover)
{
	int i;

	fprintf(fi, "echarts.init(document.getElementById(\"cover\")).setOption({\n");
	fprintf(fi, "tooltip : {\n");
	fprintf(fi, "trigger: 'axis',\n");
	fprintf(fi, "axisPointer : {\n");
	fprintf(fi, "type : 'shadow'\n");
	fprintf(fi, "}\n");
	fprintf(fi, "},\n");
	fprintf(fi, "title: {\n");
	fprintf(fi, "text: 'Coverage Histogram',\n");
	fprintf(fi, "left: 'center',\n");
	fprintf(fi, "top: 20\n");
	fprintf(fi, "},\n");
	fprintf(fi, "yAxis: {\n");
	fprintf(fi, "name : 'Total base (Mpb)',\n");
	fprintf(fi, "nameTextStyle: {\n");
	fprintf(fi, "padding: [0, 20, 0, 0]\n");
	fprintf(fi, "},\n");
	fprintf(fi, "axisLine: {\n");
	fprintf(fi, "onZero: false\n");
	fprintf(fi, "},\n");
	fprintf(fi, "type: 'value'\n");
	fprintf(fi, "},\n");
	fprintf(fi, "xAxis: {\n");
	fprintf(fi, "type: 'category',\n");
	fprintf(fi, "data: [");
	for (i = 0; i < N_COVER; ++i) {
		fprintf(fi, "%d", i);
		if (i < N_COVER - 1)
			fprintf(fi, ", ");
	}
	fprintf(fi, "],\n");
	fprintf(fi, "axisLabel: {\n");
	fprintf(fi, "interval: 4\n");
	fprintf(fi, "},\n");
	fprintf(fi, "axisTick: {\n");
	fprintf(fi, "alignWithLabel: true\n");
	fprintf(fi, "}\n");
	fprintf(fi, "},\n");
	fprintf(fi, "series: [{\n");
	fprintf(fi, "type: 'bar',\n");
	fprintf(fi, "barWidth: '8',\n");
	fprintf(fi, "barGap: '2',\n");
	fprintf(fi, "data: [");
	for (i = 0; i < N_COVER; ++i) {
		fprintf(fi, "%ld", cover[i]);
		if (i < N_COVER - 1)
			fprintf(fi, ", ");
	}
	fprintf(fi, "]\n");
	fprintf(fi, "}]\n");
	fprintf(fi, "});\n");
}

void plot_mlc_len(const int64_t *mlc_len)
{
	int i;

	fprintf(fi, "echarts.init(document.getElementById(\"mlc_len\")).setOption({\n");
	fprintf(fi, "tooltip : {\n");
	fprintf(fi, "trigger: 'axis',\n");
	fprintf(fi, "axisPointer : {\n");
	fprintf(fi, "type : 'shadow'\n");
	fprintf(fi, "}\n");
	fprintf(fi, "},\n");
	fprintf(fi, "title: {\n");
	fprintf(fi, "text: 'Molecule Length Distribution',\n");
	fprintf(fi, "left: 'center',\n");
	fprintf(fi, "top: 20\n");
	fprintf(fi, "},\n");
	fprintf(fi, "yAxis: {\n");
	fprintf(fi, "name : 'Total DNA mass',\n");
	fprintf(fi, "nameLocation: 'center',\n");
	fprintf(fi, "axisLabel: {\n");
	fprintf(fi, "show: false\n");
	fprintf(fi, "},\n");
	fprintf(fi, "axisTick: {\n");
	fprintf(fi, "show: false\n");
	fprintf(fi, "},\n");
	fprintf(fi, "nameTextStyle: {\n");
	fprintf(fi, "padding: [0, 0, 20, 0]\n");
	fprintf(fi, "},\n");
	fprintf(fi, "axisLine: {\n");
	fprintf(fi, "onZero: false\n");
	fprintf(fi, "},\n");
	fprintf(fi, "type: 'value'\n");
	fprintf(fi, "},\n");
	fprintf(fi, "xAxis: {\n");
	fprintf(fi, "type: 'category',\n");
	fprintf(fi, "data: [");
	for (i = 0; i < N_MLC_LEN; ++i) {
		fprintf(fi, "%d", i * 4);
		if (i < N_MLC_LEN - 1)
			fprintf(fi, ", ");
	}
	fprintf(fi, "],\n");
	fprintf(fi, "name : 'Molecule length',\n");
	fprintf(fi, "nameLocation: 'center',\n");
	fprintf(fi, "nameTextStyle: {\n");
	fprintf(fi, "padding: [20, 0, 0, 0]\n");
	fprintf(fi, "},\n");
	fprintf(fi, "axisLabel: {\n");
	fprintf(fi, "interval: 4\n");
	fprintf(fi, "},\n");
	fprintf(fi, "axisTick: {\n");
	fprintf(fi, "alignWithLabel: true\n");
	fprintf(fi, "}\n");
	fprintf(fi, "},\n");
	fprintf(fi, "series: [{\n");
	fprintf(fi, "type: 'bar',\n");
	fprintf(fi, "barWidth: '8',\n");
	fprintf(fi, "barGap: '2',\n");
	fprintf(fi, "data: [");
	for (i = 0; i < N_MLC_LEN; ++i) {
		fprintf(fi, "%ld", mlc_len[i]);
		if (i < N_MLC_LEN - 1)
			fprintf(fi, ", ");
	}
	fprintf(fi, "]\n");
	fprintf(fi, "}]\n");
	fprintf(fi, "});\n");
}

void plot_mlc_cover(const int64_t *mlc_cover)
{
	int i;

	fprintf(fi, "echarts.init(document.getElementById(\"mlc_cover\")).setOption({\n");
	fprintf(fi, "tooltip : {\n");
	fprintf(fi, "trigger: 'axis',\n");
	fprintf(fi, "axisPointer : {\n");
	fprintf(fi, "type : 'shadow'\n");
	fprintf(fi, "}\n");
	fprintf(fi, "},\n");
	fprintf(fi, "title: {\n");
	fprintf(fi, "text: 'Read Cloud Cover Distribution',\n");
	fprintf(fi, "left: 'center',\n");
	fprintf(fi, "top: 20\n");
	fprintf(fi, "},\n");
	fprintf(fi, "yAxis: {\n");
	fprintf(fi, "name : 'Total molecules',\n");
	fprintf(fi, "nameLocation: 'center',\n");
	fprintf(fi, "nameTextStyle: {\n");
	fprintf(fi, "padding: [0, 0, 25, 0]\n");
	fprintf(fi, "},\n");
	fprintf(fi, "axisLine: {\n");
	fprintf(fi, "onZero: false\n");
	fprintf(fi, "},\n");
	fprintf(fi, "type: 'value'\n");
	fprintf(fi, "},\n");
	fprintf(fi, "xAxis: {\n");
	fprintf(fi, "type: 'category',\n");
	fprintf(fi, "data: [");
	for (i = 0; i < N_MLC_COVER; ++i) {
		if (i == 0)
			fprintf(fi, "0");
		else if (i < 10)
			fprintf(fi, "0.0%d", i);
		else
			fprintf(fi, "0.%d", i);
		if (i < N_MLC_COVER - 1)
			fprintf(fi, ", ");
	}
	fprintf(fi, "],\n");
	fprintf(fi, "axisLabel: {\n");
	fprintf(fi, "interval: 4\n");
	fprintf(fi, "},\n");
	fprintf(fi, "axisTick: {\n");
	fprintf(fi, "alignWithLabel: true\n");
	fprintf(fi, "}\n");
	fprintf(fi, "},\n");
	fprintf(fi, "series: [{\n");
	fprintf(fi, "type: 'bar',\n");
	fprintf(fi, "barWidth: '8',\n");
	fprintf(fi, "barGap: '2',\n");
	fprintf(fi, "data: [");
	for (i = 0; i < N_MLC_COVER; ++i) {
		fprintf(fi, "%ld", mlc_cover[i]);
		if (i < N_MLC_COVER - 1)
			fprintf(fi, ", ");
	}
	fprintf(fi, "]\n");
	fprintf(fi, "}]\n");
	fprintf(fi, "});\n");
}

void plot_destroy()
{
	fprintf(fi, "</script>\n");
	fclose(fi);
}