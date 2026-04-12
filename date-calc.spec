Name: date-calc
Version: 0.1
Release: alt1
Summary: Date calculator
License: GPL-3.0-or-later
Group: System/X11

%description
%summary

%install
mkdir -p %buildroot/usr/bin
cp date-calc %buildroot/usr/bin
mkdir -p %buildroot/usr/share/applications
cp date-calc.desktop %buildroot/usr/share/applications/date-calc.desktop

%files
/usr/bin/date-calc
/usr/share/applications/date-calc.desktop
