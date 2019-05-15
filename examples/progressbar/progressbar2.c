#include <gtk/gtk.h>
typedef struct _ProgressData {
  GtkWidget *window;
  GtkWidget *pbar;
  int timer;
  gboolean activity_mode;
} ProgressData;
/* Обновляем значение шкалы выполнения
 * для отображения некоторых действий */
gint progress_timeout( gpointer data )
{
  ProgressData *pdata = (ProgressData *)data;
  gdouble new_val;
  
  if (pdata->activity_mode) 
    gtk_progress_bar_pulse (GTK_PROGRESS_BAR (pdata->pbar));
  else 
    {
      /* Вычисляем значение шкалы выполнения используя
       * установленные значения диапазона в объекте регулирования (adjustment object) */
      
      new_val = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR (pdata->pbar)) + 0.01;
      
      if (new_val > 1.0)
        new_val = 0.0;
      
      /* Устанавливаем новые значения */
      gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (pdata->pbar), new_val);
    }
  
  /* Для продолжения вызова функции перерыва 
   * (timeout function) необходимо вернуть значение TRUE */
  return TRUE;
} 
/* Обратный вызов переключателя для отображения текста в колее шкалы выполнения */
void toggle_show_text( GtkWidget    *widget,
                       ProgressData *pdata )
{
  const gchar *text;
  
  text = gtk_progress_bar_get_text (GTK_PROGRESS_BAR (pdata->pbar));
  if (text && *text)
    gtk_progress_bar_set_text (GTK_PROGRESS_BAR (pdata->pbar), "");
  else 
    gtk_progress_bar_set_text (GTK_PROGRESS_BAR (pdata->pbar), "любой текст");
}
/* Переключатель активного состояния шкалы выполнения */
void toggle_activity_mode( GtkWidget    *widget,
                           ProgressData *pdata )
{
  pdata->activity_mode = !pdata->activity_mode;
  if (pdata->activity_mode) 
      gtk_progress_bar_pulse (GTK_PROGRESS_BAR (pdata->pbar));
  else
      gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (pdata->pbar), 0.0);
}
 
/* Переключатель ориентации шкалы выполнения */
void toggle_orientation( GtkWidget    *widget,
                         ProgressData *pdata )
{
  switch (gtk_progress_bar_get_orientation (GTK_PROGRESS_BAR (pdata->pbar))) {
  case GTK_PROGRESS_LEFT_TO_RIGHT:
    gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (pdata->pbar), 
                                      GTK_PROGRESS_RIGHT_TO_LEFT);
    break;
  case GTK_PROGRESS_RIGHT_TO_LEFT:
    gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (pdata->pbar), 
                                      GTK_PROGRESS_LEFT_TO_RIGHT);
    break;
  }
}
 
/* Очистка выделенной памяти и удаление таймера */
void destroy_progress( GtkWidget     *widget,
                       ProgressData *pdata)
{
    gtk_timeout_remove (pdata->timer);
    pdata->timer = 0;
    pdata->window = NULL;
    g_free (pdata);
    gtk_main_quit ();
}
int main( int   argc,
          char *argv[])
{
    ProgressData *pdata;
    GtkWidget *align;
    GtkWidget *separator;
    GtkWidget *table;
    GtkWidget *button;
    GtkWidget *check;
    GtkWidget *vbox;
    GtkWidget *hbox;
    gtk_init (&argc, &argv);
    /* Выделение памяти для данных размещенных в обратных вызовах */
    pdata = g_malloc (sizeof (ProgressData));
  
    pdata->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_resizable (GTK_WINDOW (pdata->window), TRUE);
    g_signal_connect (G_OBJECT (pdata->window), "destroy",
                      G_CALLBACK (destroy_progress),
                      (gpointer) pdata);
    gtk_window_set_title (GTK_WINDOW (pdata->window), "GtkProgressBar");
    gtk_container_set_border_width (GTK_CONTAINER (pdata->window), 5);
    vbox = gtk_vbox_new (FALSE, 5);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
    gtk_container_add (GTK_CONTAINER (pdata->window), vbox);
    gtk_widget_show (vbox);
    // горизонтальный бокс для progressbar
    hbox = gtk_hbox_new (TRUE, 5);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
    gtk_widget_show (hbox);
  
    /* Создаём объект выравнивания */
    align = gtk_alignment_new (1.0, 1.0, 1.0, 1.0);
    gtk_box_pack_start (GTK_BOX (hbox), align, FALSE, TRUE, 5);
    gtk_widget_show (align);
    /* Создаём GtkProgressBar */
    pdata->pbar = gtk_progress_bar_new ();
    gtk_container_add (GTK_CONTAINER (align), pdata->pbar);
    //gtk_box_pack_start (GTK_BOX (align), pdata->pbar, FALSE, TRUE, 5);
    gtk_widget_show (pdata->pbar);
    /* Добавляем обратный вызов таймера для обновления значения шкалы выполнения */
    pdata->timer = gtk_timeout_add (100, progress_timeout, pdata);
    separator = gtk_hseparator_new ();
    gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, FALSE, 0);
    gtk_widget_show (separator);
    /* ряды, колонки, homogeneous */
    table = gtk_table_new (2, 3, FALSE);
    gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, TRUE, 0);
    gtk_widget_show (table);
    /* Добавляем кнопку контроля для отображения текста в колее */
    check = gtk_check_button_new_with_label ("Показать текст");
    gtk_table_attach (GTK_TABLE (table), check, 0, 1, 0, 1,
                      GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL,
                      5, 5);
    g_signal_connect (G_OBJECT (check), "clicked",
                      G_CALLBACK (toggle_show_text),
                      (gpointer) pdata);
    gtk_widget_show (check);
    /* Добавляем контроль-кнопку активного режима */
    check = gtk_check_button_new_with_label ("Активный режим");
    gtk_table_attach (GTK_TABLE (table), check, 0, 1, 1, 2,
                      GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL,
                      5, 5);
    g_signal_connect (G_OBJECT (check), "clicked",
                      G_CALLBACK (toggle_activity_mode),
                      (gpointer) pdata);
    gtk_widget_show (check);
    /* Добавляем контроль-кнопку переключателя активного режима */
    check = gtk_check_button_new_with_label ("Справа на лево");
    gtk_table_attach (GTK_TABLE (table), check, 0, 1, 2, 3,
                      GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL,
                      5, 5);
    g_signal_connect (G_OBJECT (check), "clicked",
                      G_CALLBACK (toggle_orientation),
                      (gpointer) pdata);
    gtk_widget_show (check);
    // горизонтальный бокс для кнопки "закрыть"
    hbox = gtk_hbox_new (FALSE, 5);
    gtk_box_pack_end (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
    gtk_widget_show (hbox);
    /* Добавляем кнопку выхода из программы */
    button = gtk_button_new_with_label ("закрыть");
    g_signal_connect_swapped (G_OBJECT (button), "clicked",
                              G_CALLBACK (gtk_widget_destroy),
                              G_OBJECT (pdata->window));
    gtk_box_pack_end (GTK_BOX (hbox), button, FALSE, TRUE, 0);
    /* Создаем активную кнопку по умолчанию. */
    GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
    /* Устанавливаем перехват для кнопки по умолчанию. 
     * Простое нажатие клавиши "Enter" активизирует кнопку. */
    gtk_widget_grab_default (button);
    gtk_widget_show (button);
    gtk_widget_show (pdata->window);
    gtk_main ();
    
    return 0;
}
