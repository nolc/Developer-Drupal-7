<?php

function menufun_greeting($first_name = '', $last_name = '', $from_first_name='',
$from_last_name='') {
  return t('Hello @first_name @last_name from @from_first_name @from_last_name',
    array('@first_name' => $first_name, '@last_name' => $last_name, '@from_first_name' =>
$from_first_name, '@from_last_name' => $from_last_name));
}


function menufun_farewell() {

  return t('Goodbye');
}


/**
 * Title callback.
 */
function menufun_title() {
	//cuando el titulo no es muy amigable usamos esta funcion  para definir uno
drupal_set_title(t('Titulo de pagina'));

  $now = format_date(time());
  return t('It is now @time', array('@time' => $now));
}