#!/usr/bin/perl

use warnings;
use strict;
use File::Temp qw(tempfile tempdir);
use Gtk2 -init;
use utf8;


my $bin_latex;
my $bin_pdfviewer;

if ($^O eq "linux") {
	$bin_latex = "/usr/bin/latex";
	$bin_pdfviewer = "/usr/bin/evince";
} else {
	$bin_latex = "latex";
	$bin_pdfviewer = '"C:\Programme\Adobe\Acrobat 7.0\Reader\AcroRd32.exe"';
}

my @grades = ('++', '+', '0', '-', '--', 'Ohne');
my @check_class = ( 'Fachkenntnisse', 'Fertigkeiten', 'Arbeitssicheres Verhalten', 'Zuverlässigkeit', 'Lern- und Arbeitsbereitschaft', 'Umgang mit Einrichtungen / Arbeitsmitteln', 'Teamfähigkeit', 'Verantwortungsbewustsein' );

my $default_top_pos = "15";
my $default_left_pos = "15";

my $check_space = "64";
my $text_space = "7";

my @kids;

# Abstände Zwischen den "X" Zeilen
my %check_pos = (
	'Fachkenntnisse'				=> "0",
	'Fertigkeiten'					=> "0",
	'Arbeitssicheres Verhalten'			=> "0",
	'Zuverlässigkeit'				=> "14",
	'Lern- und Arbeitsbereitschaft'			=> "0",
	'Umgang mit Einrichtungen / Arbeitsmitteln'	=> "0",
	'Teamfähigkeit'					=> "6",
	'Verantwortungsbewustsein'			=> "0",
);

# Horizontale Positionen der Xe
my %check_grades_pos = (
	'++'	=> "126",
	'+'	=> "139",
	'0'	=> "152",
	'-'	=> "165",
	'--'	=> "178",
);

my @text_class = qw( Fertigkeiten Kenntnisse Zusammenarbeit Auffassungsgabe Transfervermögen Sorgfalt Lerntempo Interesse Zuverlässigkeit Ausdauer );
# my @text_class = qw( Kenntnisse Fertigkeiten Zuverlässigkeit Interesse Sorgfalt Zusammenarbeit );
my %text = (
	"Fertigkeiten" => {
		"++"	=> "Verfügt über einen sehr hohen Fertigkeitsgrad. Führt die übertragenen Tätigkeiten mit großer Geschicklichkeit durch. ",
		"+"	=> "Verfügt über einen hohen Fertigkeitsgrad. Arbeitet sicher und geschickt. ", 
		"0"	=> "Die Fertigkeiten ermöglichen eine zufriedenstellende Arbeitsausführung. Ist selten unsicher. ", 
		"-"	=> "Der erforderliche Fertigkeitsgrad wird nicht immer erreicht. Die Arbeitsausführung wird dadurch erschwert. ", 
		"--"	=> "Kann die Anforderungen an Fertigkeiten kaum erfüllen. Ist bei vielen Tätigkeiten unsicher und ungeschickt. ",
		"Ohne"	=> ""
	},
	"Kenntnisse" => {
		"++"	=> "Verfügt über besonders umfangreiche Fachkenntnisse und erkennt sicher Zusammenhänge. ", 
		"+"	=> "Verfügt über umfangreiche Fachkenntnisse. Kann Zusammenhänge herstellen. ", 
		"0"	=> "Besitzt die erforderlichen Fachkenntnisse, um die übertragenen Aufgaben zufriedenstellend ausführen zu können. ", 
		"-"	=> "Die erforderlichen Fachkenntnisse sind nicht immer vorhanden. Fehlendes Wissen erschwert den Arbeits- und damit auch den Ausbildungsablauf. ", 
		"--"	=> "Verfügt kaum über die erforderlichen Fachkenntnisse, ist häufig auf Erklärungen, Hilfen und Ratschläge angewiesen. ",
		"Ohne"	=> ""
	},
	"Zusammenarbeit" => {
		"++"	=> "Zeigt besonderes Einfühlungsvermögen im Umgang mit anderen. Gute Zusammenarbeit und Hilfsbereitschaft. Aufgeschlossen und fair. ", 
		"+"	=> "Hat gutes Einfühlungsvermögen im Umgang mit anderen. Ist hilfsbereit und fähig zu guter Zusammenarbeit. ", 
		"0"	=> "Zeigt in der Regel Einfühlungsvermögen im Umgang mit anderen. Hat den Willen zu Hilfsbereitschaft und Zusammenarbeit. ", 
		"-"	=> "Zeigt Unsicherheiten im Umgang mit anderen, wodurch eine problemlose Zusammenarbeit erschwert wird. Arbeitet, von Ausnahmefällen abgesehen, in der Gruppe mit. ", 
		"--"	=> "Zeigt ungenügendes Einfühlungsvermögen im Umgang mit anderen. Kein ausgeprägtes Gefühl für Zusammenarbeit. Arbeitet lieber allein. ",
		"Ohne"	=> ""
	},
	"Auffassungsgabe" => {
		"++"	=> "Auch schwierige Sachverhalte werden schnell begriffen, Zusammenhänge klar erkannt, Einzeldaten gewichtet und zugeordnet. ",
		"+"	=> "Schnelle Auffassung. Der Kern einer Sache wird rasch begriffen. Ist in der Lage, Wesentliches vom Unwesentlichen zu unterscheiden. ", 
		"0"	=> "Inhalt und Bedeutung eines Sachverhalts werden erfaßt. Das Begriffene wird sachlich richtig eingeordnet. ", 
		"-"	=> "Anleitungen bzw. wiederholte Erklärungen sind notwendig, damit Lerninhalte und -situationen verstanden werden. ", 
		"--"	=> "Lerninhalte und -situationen werden selbst nach eingehender, wiederholter Erklärung nur unvollkommen verstanden. ",
		"Ohne"	=> ""
	},
	"Transfervermögen" => {
		"++"	=> "Sichere und richtige Übertragung gewonnener Erkenntnisse. ", 
		"+"	=> "Gewonnene Erkenntnisse werden übertragen. ", 
		"0"	=> "Gewonnene Erkenntnisse werden meist übertragen. ", 
		"-"	=> "Kann gewonnene Erkenntnisse nur vereinzelt übertragen. ", 
		"--"	=> "Gewonnene Erkenntnisse werden nicht übertragen ",
		"Ohne"	=> ""
	},
	"Sorgfalt" => {
		"++"	=> "Arbeitet stets planvoll und mit großer Sorgfalt. Arbeitsergebnisse liegen immer im Bereich der Qualitätsanforderungen. ",
		"+"	=> "Arbeitet planvoll. Ist sorgfältig in der Arbeitsausführung. Arbeitsergebnisse liegen nur selten außerhalb der gestellten Qualitätsanforderungen. ", 
		"0"	=> "Es wird im allgemeinen planvoll und sorgfältig gearbeitet. Arbeitsergebnisse liegen zum größten Teil im Bereich der Qualitätsanforderungen. ", 
		"-"	=> "Planmäßigkeit und Sorgfalt bei der Arbeitsausführung lassen zu wünschen übrig. Arbeitsergebnisse entsprechen häufig nicht den gestellten Qualitätsanforderungen. ", 
		"--"	=> "Übertragene Aufgaben werden nicht planvoll und sorgfältig durchgeführt. Erreicht kein ausreichendes Arbeitsergebnis. ",
		"Ohne"	=> ""
	},
	"Lerntempo" => {
		"++"	=> "Fertigkeiten werden besonders rasch beherrscht. Das Lerntempo ist außerordentlich hoch. Gestellte Aufgaben werden immer schneller erledigt, als der Ausbildungsstand erwarten läßt. ", 
		"+"	=> "Fertigkeiten werden rasch beherrscht. Das Lerntempo ist hoch. Gestellte Aufgaben werden häufig schneller erledigt, als der Ausbildungsstand erwarten läßt. ", 
		"0"	=> "Fertigkeiten werden nach Übung beherrscht. Das Lerntempo ist ausreichend. Gestellte Aufgaben werden in einer dem Ausbildungsstand angemessenen Zeit bewältigt. ", 
		"-"	=> "Fertigkeiten werden meist erst nach längerer Übung beherrscht. Das Lerntempo ist nicht immer ausreichend. Benötigt für die gestellten Aufgaben meist mehr Zeit als vorgesehen. ", 
		"--"	=> "Fertigkeiten werden auch nach längerer Übung kaum beherrscht. Das Lerntempo ist gering. Kommt bei der Ausführung der gestellten Aufgaben mit der vorgesehenen Zeit nicht aus. ",
		"Ohne"	=> ""
	},
	"Interesse" => {
		"++"	=> "Zeigt außergewöhnliches Interesse. Besonders ausgeprägte Initiative. Scheut auch vor schwierigen Aufgaben nicht zurück. Sehr zielstrebig. ", 
		"+"	=> "Zeigt Interesse und Initiative. Beteiligt sich an der Lösung auch schwieriger Aufgaben. ", 
		"0"	=> "Ist Interessiert und aufgeschlossen. Setzt seine Fähigkeiten effektiv ein. Braucht nur selten Anregungen bei schwierigen Aufgaben. ", 
		"-"	=> "Zeigt nicht immer Interesse und Initiative. Bedarf der Anregungen. ", 
		"--"	=> "Zeigt kaum Interesse und  Initiative. Meidet schwierige Aufgaben. Bedarf ständiger Anregungen. ",
		"Ohne"	=> ""
	},
	"Zuverlässigkeit" => {
		"++"	=> "Ist sehr zuverlässig und verantwortungsbewußt in der Erledigung der gestellten Aufgaben und insbesondere bei der Einhaltung von Vorschriften, Anweisungen und Terminen. ", 
		"+"	=> "Ist zuverlässig und verantwortungsbewußt in der Erledigung gestellter Aufgaben. Vorschriften, Anweisungen und Termine werden eingehalten. ", 
		"0"	=> "Übertragene Aufgaben werden im allgemeinen zuverlässig durchgeführt. In der Regel werden Vorschriften, Anweisungen und Termine eingehalten. ", 
		"-"	=> "Zuverlässigkeit läßt zu wünschen übrig. Vorschriften und Anweisungen werden oft nicht ausreichend beachtet. Es gibt Schwierigkeiten bei der Einhaltung von Terminen. ", 
		"--"	=> "Vorschriften und Anweisungen werden nur ungenügend beachtet ist nicht zuverlässig bei der Einhaltung von Terminen ",
		"Ohne"	=> ""
	},
	"Ausdauer" => {
		"++"	=> "Ist außerordentlich ausdauernd auch unter erschwerten Bedingungen. ", 
		"+"	=> "Ist ausdauernd. Gelegentliche Schwierigkeiten werden überwunden. ", 
		"0"	=> "Ist im allgemeinen beharrlich und beständig. ", 
		"-"	=> "Ist unterschiedlich ausdauernd. Schwierigkeiten werden nur mühsam überwunden. ", 
		"--"	=> "Weniger beharrlich und beständig. Gibt bei Schwierigkeiten schnell auf. ",
		"Ohne"	=> ""
	}
);


# Latex grundgerüst für die Beurteilung
my $latex_frame = <<'EOF';
\batchmode

\documentclass[11pt]{article}

\usepackage{ngerman}
\usepackage[latin1]{inputenc}

\usepackage{vmargin}
\setpapersize{A4}
\setmargins{VAR_LEFT_POSmm}{VAR_TOP_POSmm}% % linker & oberer Rand
           {184mm}{250mm}%   % Textbreite und -hoehe
           {12pt}{25pt}%   % Kopfzeilenhoehe und -abstand
           {0pt}{30pt}%    % \footheight (egal) und Fusszeilenabstand

\parindent0pt
\pagestyle{empty}


\begin{document}
\vspace*{VAR_CHECK_SPACEmm}
VAR_CHECK
\vspace*{VAR_TEXT_SPACEmm}
VAR_TEXT
\end{document}
EOF


my $true = 1;
my $false = 0;


my %combobox;
my %checkbutton;

my $check_class_table = Gtk2::Table->new(@check_class + 1, @grades + 1, $false);
$check_class_table->show();

for (my $i = 0; $i < @grades; $i++) {
	my $label = Gtk2::Label->new($grades[$i]);
	$label->show();
	$check_class_table->attach_defaults($label, $i + 1, $i + 2, 0, 1);
}

my $n = 1;
foreach my $check_class (@check_class) {
	my $label = Gtk2::Label->new($check_class);
	$check_class_table->attach_defaults($label, 0, 1, $n, $n + 1);
	$label->set_justify('left');
	$label->show();

	my $i = 1;
	my $group = undef;
	foreach (@grades) {
		$checkbutton{$check_class}{$_} = Gtk2::RadioButton->new($group);
		$group = $checkbutton{$check_class}{$_}->get_group();
		$checkbutton{$check_class}{$_}->show();
		$check_class_table->attach_defaults($checkbutton{$check_class}{$_}, $i, $i + 1, $n, $n + 1);

		$i++;
	}

	$n++
}


my $text_class_table = Gtk2::Table->new(sprintf("%.f", @text_class / 2) , 4 , $true );		# sprintf zum runden verwendet
$text_class_table->show();

$n = 0;
foreach my $text_class (@text_class) {
	my $label = Gtk2::Label->new($text_class);
	$text_class_table->attach_defaults($label, $n % 2 * 2 , $n % 2 * 2 + 1, $n / 2, $n / 2 + 1);
	$label->show();

	$combobox {$text_class} = Gtk2::ComboBox->new_text();
	$text_class_table->attach_defaults( $combobox{$text_class}, $n % 2 * 2 + 1, $n % 2 * 2 + 2, $n / 2, $n / 2 + 1 );
	foreach (@grades) { $combobox{$text_class}->append_text($_); }
	$combobox{$text_class}->set_active(0);
	$combobox{$text_class}->show();

	$n++;
}



my $v_offset_label = Gtk2::Label->new("Vertikaler Offset:");
$v_offset_label->show();
my $v_offset_button = Gtk2::SpinButton->new_with_range (-10, 10, 1);
$v_offset_button->set_value(0);
$v_offset_button->show();
my $h_offset_label = Gtk2::Label->new("Horizontaler Offset:");
$h_offset_label->show();
my $h_offset_button = Gtk2::SpinButton->new_with_range (-10, 10, 1);
$h_offset_button->set_value(0);
$h_offset_button->show();

my $offset_box = Gtk2::HBox->new();
$offset_box->pack_start($v_offset_label, $false, $false, 0);
$offset_box->pack_start($v_offset_button, $false, $false, 0);
$offset_box->pack_start($h_offset_label, $false, $false, 0);
$offset_box->pack_start($h_offset_button, $false, $false, 0);
$offset_box->show();

# Erstelle Textfeld
my $buffer = Gtk2::TextBuffer->new();
$buffer->set_text('Zum erstellen des Textes [Text zusammenstellen] drücken!');

my $text_view = Gtk2::TextView->new_with_buffer($buffer);
$text_view->set_wrap_mode('word');
$text_view->set_size_request(-1, 150);
$text_view->show();

# Schliessen taste
my $close_button = Gtk2::Button->new('Schliessen');
$close_button->signal_connect( "clicked", sub { Gtk2->main_quit(); } );
$close_button->show();

# Zusammenstellen taste
my $stack_button = Gtk2::Button->new('Text zusammenstellen');
$stack_button->signal_connect( "clicked", sub { stack(); } );
$stack_button->show();

# Erstellen taste
my $build_button = Gtk2::Button->new('Erstelle PDF');
$build_button->signal_connect( "clicked", sub { build(); } );
$build_button->show();

# Buttonleiste zusammenstellen
my $button_box = Gtk2::HBox->new();
$button_box->pack_end($close_button, $false, $false, 0);
$button_box->pack_start($stack_button, $false, $false, 0);
$button_box->pack_start($build_button, $false, $false, 0);
$button_box->show();

# Fensterlayout zusammenstellen
my $window_layout = Gtk2::VBox->new();
$window_layout->pack_start($check_class_table, $false, $false, 0);
$window_layout->pack_start($text_class_table, $false, $false, 0);
$window_layout->pack_start($offset_box, $false, $false, 0);
$window_layout->pack_start($text_view, $true, $true, 0);
$window_layout->pack_end($button_box, $false, $false, 0);
$window_layout->set_border_width(5);
$window_layout->show();

# Fenster festlegen
my $window = Gtk2::Window->new( "toplevel" );
$window->signal_connect( "delete_event", sub { quit(); } );
$window->set_position('center');
$window->set_title('Beurteilung');
$window->add($window_layout);
$window->show();


Gtk2->main();

# Beende und Warte auf alle Kindprozesse
foreach my $kid (@kids) {
	kill 9, $kid;
	waitpid $kid, 0;
}

exit(0);


sub stack()
{
	my $text;
	foreach (@text_class) { $text .= $text{$_}{$combobox{$_}->get_active_text}; }
	$buffer->set_text($text);
}



sub build()
{

	my $text = $buffer->get_text($buffer->get_start_iter(), $buffer->get_end_iter(), $false);

	my $check;
	foreach my $check_class (@check_class) {
		foreach (@grades) {
			$check .= "\\\\[$check_pos{$check_class}mm]\\hspace*{$check_grades_pos{$_}mm}X\n" if $checkbutton{$check_class}{$_}->get_active();
		}
	}

	my $left_pos = $default_left_pos + $h_offset_button->get_value_as_int();
	my $top_pos = $default_top_pos + $v_offset_button->get_value_as_int();



	my$latex= $latex_frame;
	$latex =~ s/VAR_LEFT_POS/$left_pos/;
	$latex =~ s/VAR_TOP_POS/$top_pos/;
	$latex =~ s/VAR_CHECK_SPACE/$check_space/ ;
	$latex =~ s/VAR_CHECK/$check/ ;
	$latex =~ s/VAR_TEXT_SPACE/$text_space/ ;
	$latex =~ s/VAR_TEXT/$text/ ;
	
	my $dir = tempdir(CLEANUP => 1);
	my ($tmp_fh, $tmp_name) = tempfile(DIR => $dir, SUFFIX => ".tex");

	print $tmp_fh $latex;
	close $tmp_fh;
		
	system "$bin_latex", "-output-format=pdf", "-output-directory=$dir", "$tmp_name";
	(my $pdf_name = $tmp_name) =~ s/tex$/pdf/;
	
	my $pid;
	if ($pid = fork) {
		push @kids, $pid;
		print "Neuer Kindprozess $pid\n";
	} elsif ($pid == 0) {
		exec "$bin_pdfviewer", "$pdf_name" ; 
	}

}


sub quit()
{
	Gtk2->main_quit();
}