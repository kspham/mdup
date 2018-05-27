#include "plot.h"

static FILE *fi_data;

void plot_data_init(const char *file_path)
{
	fi_data = fopen(file_path, "w");
	fprintf(fi_data, "var plotData = {\n");
}

void plot_data_destroy()
{
	fprintf(fi_data, "}\n");
	fclose(fi_data);
}

void plot_render(const char *file_path)
{
	FILE *fi_plot = fopen(file_path, "w");
	fprintf(fi_plot,
		"<!DOCTYPE html>\n"
		"<meta charset=\"utf-8\">\n"
		"<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\" type=\"text/css\" />\n"
		"<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>\n"
		"<script src=\"https://cdnjs.cloudflare.com/ajax/libs/echarts/4.1.0/echarts.min.js\"></script>\n"
		"\n"
		"<body>\n"
		"<div id=\"readCover\" style=\"width: 1000px; height: 500px; margin: auto\"></div>\n"
		"<div id=\"mlcLen\" style=\"width: 1000px; height: 500px; margin: auto\"></div>\n"
		"<div id=\"mlcCover\" style=\"width: 1000px; height: 500px; margin: auto\"></div>\n"
		"\n"
		"<script src=\"data.js\"></script>"
		"\n"
		"<script>\n"
		"echarts.init(document.getElementById(\"readCover\")).setOption({\n"
		"    tooltip : {\n"
		"        trigger: 'axis',\n"
		"        axisPointer : {\n"
		"            type : 'shadow'\n"
		"        }\n"
		"    },\n"
		"    title: {\n"
		"        text: 'Coverage Histogram',\n"
		"        left: 'center',\n"
		"        top: 20\n"
		"    },\n"
		"    yAxis: {\n"
		"        name : 'Total base (Mpb)',\n"
		"        nameTextStyle: {\n"
		"            padding: [0, 20, 0, 0]\n"
		"        },\n"
		"        axisLine: {\n"
		"            onZero: false\n"
		"        },\n"
		"        type: 'value'\n"
		"    },\n"
		"    xAxis: {\n"
		"        type: 'category',\n"
		"        name : 'Coverage',\n"
		"        nameLocation: 'center',\n"
		"        nameTextStyle: {\n"
		"            padding: [20, 0, 0, 0]\n"
		"        },\n"
		"        data: plotData.readCoverXaxis,\n"
		"        axisLabel: {\n"
		"            interval: 4\n"
		"        },\n"
		"        axisTick: {\n"
		"            alignWithLabel: true\n"
		"        }\n"
		"    },\n"
		"    series: [{\n"
		"        type: 'bar',\n"
		"        barWidth: '8',\n"
		"        barGap: '2',\n"
		"        data: plotData.readCoverSeries\n"
		"    }]\n"
		"});\n"
		"\n"
		"echarts.init(document.getElementById(\"mlcLen\")).setOption({\n"
		"    tooltip : {\n"
		"        trigger: 'axis',\n"
		"        axisPointer : {\n"
		"            type : 'shadow'\n"
		"        }\n"
		"    },\n"
		"    title: {\n"
		"        text: 'Distribution of Molecule Length',\n"
		"        left: 'center',\n"
		"        top: 20\n"
		"    },\n"
		"    yAxis: {\n"
		"        name : 'Total DNA mass',\n"
		"        nameLocation: 'center',\n"
		"        axisLabel: {\n"
		"            show: false\n"
		"        },\n"
		"        axisTick: {\n"
		"            show: false\n"
		"        },\n"
		"        nameTextStyle: {\n"
		"            padding: [0, 0, 20, 0]\n"
		"        },\n"
		"        axisLine: {\n"
		"            onZero: false\n"
		"        },\n"
		"        type: 'value'\n"
		"    },\n"
		"    xAxis: {\n"
		"        type: 'category',\n"
		"        data: plotData.mlcLenXaxis,\n"
		"        name : 'Molecule length',\n"
		"        nameLocation: 'center',\n"
		"        nameTextStyle: {\n"
		"            padding: [20, 0, 0, 0]\n"
		"        },\n"
		"        axisLabel: {\n"
		"            interval: 4\n"
		"        },\n"
		"        axisTick: {\n"
		"            alignWithLabel: true\n"
		"        }\n"
		"    },\n"
		"    series: [{\n"
		"        type: 'bar',\n"
		"        barWidth: '6',\n"
		"        barGap: '2',\n"
		"        data: plotData.mlcLenSeries\n"
		"    }]\n"
		"});\n"
		"    \n"
		"    echarts.init(document.getElementById(\"mlcCover\")).setOption({\n"
		"    tooltip : {\n"
		"        trigger: 'axis',\n"
		"        axisPointer : {\n"
		"            type : 'shadow'\n"
		"        }\n"
		"    },\n"
		"    title: {\n"
		"        text: 'Distribution of Molecule Coverage',\n"
		"        left: 'center',\n"
		"        top: 20\n"
		"    },\n"
		"    yAxis: {\n"
		"        name : 'Number of Molecules',\n"
		"        nameLocation: 'center',\n"
		"        nameTextStyle: {\n"
		"            padding: [0, 0, 40, 0]\n"
		"        },\n"
		"        axisLine: {\n"
		"            onZero: false\n"
		"        },\n"
		"        type: 'value'\n"
		"    },\n"
		"    xAxis: {\n"
		"        type: 'category',\n"
		"        data: plotData.mlcCoverXaxis,\n"
		"        name : 'Coverage',\n"
		"        nameLocation: 'center',\n"
		"        nameTextStyle: {\n"
		"            padding: [20, 0, 0, 0]\n"
		"        },\n"
		"        axisLabel: {\n"
		"            interval: 4\n"
		"        },\n"
		"        axisTick: {\n"
		"            alignWithLabel: true\n"
		"        }\n"
		"    },\n"
		"    series: [{\n"
		"        type: 'bar',\n"
		"        barWidth: '6',\n"
		"        barGap: '2',\n"
		"        data: plotData.mlcCoverSeries\n"
		"    }]\n"
		"});\n"
		"</script>\n");
	fclose(fi_plot);
}

void plot_read_cover(const int64_t *cover)
{
	int i;
	fprintf(fi_data, "    \"readCoverXaxis\": [");
	for (i = 0; i < N_COVER; ++i) {
		fprintf(fi_data, "%d", i);
		if (i < N_COVER - 1)
			fprintf(fi_data, ", ");
	}
	fprintf(fi_data, "],\n");
	fprintf(fi_data, "    \"readCoverSeries\": [");
	for (i = 0; i < N_COVER; ++i) {
		fprintf(fi_data, "%ld", cover[i]);
		if (i < N_COVER - 1)
			fprintf(fi_data, ", ");
	}
	fprintf(fi_data, "],\n");
}

void plot_mlc_len(const int64_t *mlc_len)
{
	int i;
	fprintf(fi_data, "    \"mlcLenXaxis\": [");
	for (i = 0; i < N_MLC_LEN; ++i) {
		fprintf(fi_data, "%d", i * 4);
		if (i < N_MLC_LEN - 1)
			fprintf(fi_data, ", ");
	}
	fprintf(fi_data, "],\n");
	fprintf(fi_data, "    \"mlcLenSeries\": [");
	for (i = 0; i < N_MLC_LEN; ++i) {
		fprintf(fi_data, "%ld", mlc_len[i]);
		if (i < N_MLC_LEN - 1)
			fprintf(fi_data, ", ");
	}
	fprintf(fi_data, "],\n");
}

void plot_mlc_cover(const int64_t *mlc_cover)
{
	int i;
	fprintf(fi_data, "    \"mlcCoverXaxis\": [");
	for (i = 0; i < N_MLC_COVER; ++i) {
		if (i == 0)
			fprintf(fi_data, "0");
		else if (i < 10)
			fprintf(fi_data, "0.0%d", i);
		else
			fprintf(fi_data, "0.%d", i);
		if (i < N_MLC_COVER - 1)
			fprintf(fi_data, ", ");
	}
	fprintf(fi_data, "],\n");
	fprintf(fi_data, "    \"mlcCoverSeries\": [");
	for (i = 0; i < N_MLC_COVER; ++i) {
		fprintf(fi_data, "%ld", mlc_cover[i]);
		if (i < N_MLC_COVER - 1)
			fprintf(fi_data, ", ");
	}
	/* end data */
	fprintf(fi_data, "]\n");
}