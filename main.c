#include <gtk/gtk.h>

//https://support.microsoft.com/ru-ru/windows/%D0%B2%D1%8B%D1%87%D0%B8%D1%81%D0%BB%D0%B5%D0%BD%D0%B8%D0%B5-%D0%BA%D0%BE%D0%BB%D0%B8%D1%87%D0%B5%D1%81%D1%82%D0%B2%D0%B0-%D0%B4%D0%BD%D0%B5%D0%B9-%D0%BC%D0%B5%D0%B6%D0%B4%D1%83-%D0%B4%D0%B0%D1%82%D0%B0%D0%BC%D0%B8-%D0%B2%D1%8B%D1%87%D0%B8%D1%81%D0%BB%D0%B5%D0%BD%D0%B8%D0%B5-%D0%B4%D0%B0%D1%82%D1%8B-%D0%B2-%D0%BF%D1%80%D0%BE%D1%88%D0%BB%D0%BE%D0%BC-%D0%B8%D0%BB%D0%B8-%D0%B2-%D0%B1%D1%83%D0%B4%D1%83%D1%89%D0%B5%D0%BC-c12972c8-1c93-a4e6-32d0-0067004faa43

typedef struct {
	int years;
	int months;
	int weeks;
	int days;
} DateDiff;

static int computeJD(int y, int m, int d) {
	if (m <= 2) {
		y--;
		m += 12;
	}
	int A = y / 100;
	int B = 2 - A + (A / 4);
	int X1 = 36525 * (y + 4716) / 100;
	int X2 = 306001 * (m + 1) / 10000;
	return ((X1 + X2 + d + B - 1524/*- 1524.5 */));
}

static int is_leap(int y) {
	return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

static int days_in_month(int y, int m) {
	if (m == 2 && is_leap(y))
		return 29;

	static const int dim[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	return dim[m - 1];
}

/* Число дней в календарном месяце, который непосредственно предшествует m2 в году y2 */
static int days_in_month_before(int y2, int m2) {
	if (m2 == 1)
		return days_in_month(y2 - 1, 12);
	return days_in_month(y2, m2 - 1);
}

/* <0 если a раньше b, 0 если равны, >0 если a позже b */
static int date_cmp(int ya, int ma, int da, int yb, int mb, int db) {
	if (ya != yb)
		return ya > yb ? 1 : -1;
	if (ma != mb)
		return ma > mb ? 1 : -1;
	if (da != db)
		return da > db ? 1 : -1;
	return 0;
}

DateDiff date_difference(int y1, int m1, int d1, int y2, int m2, int d2) {
	int ord = date_cmp(y1, m1, d1, y2, m2, d2);
	if (ord == 0)
		return (DateDiff){0, 0, 0, 0};
	if (ord > 0) {
		int t;

		t = y1;
		y1 = y2;
		y2 = t;

		t = m1;
		m1 = m2;
		m2 = t;

		t = d1;
		d1 = d2;
		d2 = t;
	}

	int y = y2 - y1;
	int m = m2 - m1;
	int d = d2 - d1;

	if (d < 0) {
		m--;
		d += days_in_month_before(y2, m2);
	}
	if (m < 0) {
		y--;
		m += 12;
	}

	return (DateDiff){y, m, d / 7, d % 7};
}

// GUI
GtkWidget *cal1, *cal2, *lDiff;

static void day_selected() {
	guint year1, month1, day1;
	gtk_calendar_get_date(GTK_CALENDAR(cal1), &year1, &month1, &day1);
	int jd1 = computeJD(year1, month1 + 1, day1);

	guint year2, month2, day2;
	gtk_calendar_get_date(GTK_CALENDAR(cal2), &year2, &month2, &day2);
	int diff = computeJD(year2, month2 + 1, day2) - jd1;
	if (diff < 0)
		diff = -diff;

	DateDiff dd = date_difference(year1, month1, day1, year2, month2, day2);
	char buf[200];
	snprintf(buf, sizeof(buf),
		"%i %s, %i %s, %i %s, %i %s\n"
		"(%i %s)",
		dd.years, (dd.years == 1) ? "год" : ((dd.years == 0 || dd.years >= 5) ? "лет" : "года"),
		dd.months, (dd.months == 1) ? "месяц" : ((dd.months == 0 || dd.months >= 5) ? "месяцев" : "месяца"),
		dd.weeks, (dd.weeks == 1) ? "неделя" : ((dd.weeks == 0 || dd.weeks >= 5) ? "недель" : "недели"),
		dd.days, (dd.days == 1) ? "день" : ((dd.days == 0 || dd.days >= 5) ? "дней" : "дня"),
		diff, (diff == 1) ? "день" : ((diff == 0 || diff >= 5) ? "дней" : "дня"));
	gtk_label_set_label(GTK_LABEL(lDiff), buf);
}

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);

	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(window), "Вычисление дат");
		gtk_window_set_default_size(GTK_WINDOW(window), 350, -1);
		g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
		GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
			gtk_box_pack_start(GTK_BOX(vbox), gtk_label_new("От"), FALSE, FALSE, 0);
			cal1 = gtk_calendar_new();
				g_signal_connect(cal1, "day-selected", G_CALLBACK(day_selected), NULL);
			gtk_box_pack_start(GTK_BOX(vbox), cal1, FALSE, FALSE, 0);

			gtk_box_pack_start(GTK_BOX(vbox), gtk_label_new("\nДо"), FALSE, FALSE, 0);
			cal2 = gtk_calendar_new();
				g_signal_connect(cal2, "day-selected", G_CALLBACK(day_selected), NULL);
			gtk_box_pack_start(GTK_BOX(vbox), cal2, FALSE, FALSE, 0);

			gtk_box_pack_start(GTK_BOX(vbox), gtk_label_new("\nОтличие"), FALSE, FALSE, 0);
			lDiff = gtk_label_new("");
			gtk_box_pack_start(GTK_BOX(vbox), lDiff, FALSE, FALSE, 0);
		gtk_container_add (GTK_CONTAINER (window), vbox);
	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}
