<?php

/**
 * Form builder. Configure annotations.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function anotacion_admin_settings() {
  // Get an array of node types with internal names as keys and
  // "friendly names" as values. E.g.,
  // array('page' => 'Basic Page', 'article' => 'Articles')
 $types = node_type_get_types();
  foreach($types as $node_type) {
     $options[$node_type->type] = $node_type->name;
  }

  $form['anotacion_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' =>t('Los usuarios pueden realizar anotaciones en estos tipos de contenido'),
    '#options' => $options,
    '#default_value' => variable_get('anotacion_node_types', array('page')),
    '#description' => t('Un campo de texto estará disponible en los tipos de contenido para hacer notas específicas del usuario.'),
  );

  $form['anotacion_deletion'] = array(
    '#type' => 'radios',
    '#title' => t('Annotations will be deleted'),
    '#description' => t('Select a method for deleting annotations.'),
    '#options' => array(
      t('Never'),
      t('Randomly'),
      t('After 30 days')
    ),
    '#default_value' => variable_get('anotacion_deletion', 0) // Default to Never
  );

    $form['anotacion_limit_per_node'] = array(
    '#type' => 'textfield',
    '#title' => t('Annotations per node'),
    '#description' => t('Enter the maximum number of annotations allowed per
      node (0 for no limit).'),
    '#default_value' => variable_get('anotacion_limit_per_node', 1),
    '#size' => 3
  );

  $form['#submit'][] = 'anotacion_admin_settings_submit';
  return system_settings_form($form);
}



/**
* Process annotation settings submission.
*/
function anotacion_admin_settings_submit($form, $form_state) {
  // Loop through each of the content type checkboxes shown on the form.
  foreach ($form_state['values']['anotacion_node_types'] as $key => $value)
  {
    // If the check box for a content type is unchecked, look to see wheth
    // this content type has the annotation field attached to it using the
    // field_info_instance function. If it does then we need to remove the
    // annotation field as the administrator has unchecked the box.
    if (!$value) {
      $instance = field_info_instance('node', 'anotacion', $key);
      if (!empty($instance)) {
        field_delete_instance($instance);
        watchdog("Annotation", 'Deleted annotation field from content type
          %key', array('%key' => $key));
      }
    } else {
      // If the check box for a content type  is checked, look to see whet
      // the field is associated with that content type. If not then add t
      // annotation field to the content type.
      $instance = field_info_instance('node', 'anotacion', $key);
      if (empty($instance)) {
        $instance = array(
          'field_name' => 'anotacion',
          'entity_type' => 'node',
          'bundle' => $key,
          'label' => t('Anotacion'),
          'widget_type' => 'text_textarea_with_summary',
          'settings' => array('display_summary' => TRUE),
          'display' => array(
            'default' => array(
              'type' => 'text_default',
               ),
            'teaser' => array(
              'type' => 'text_summary_or_trimmed',
            ),
        ),
      );
      $instance = field_create_instance($instance);
      watchdog('Annotation', 'Added annotation field to content type: %key',
        array('%key' => $key));
      }
    }
  } // End foreach loop.
}






/**
 * Validate annotation settings submission.
 */
function anotacion_admin_settings_validate($form, &$form_state) {
  $limit = $form_state['values']['anotacion_limit_per_node'];
  if (!is_numeric($limit)) {
    form_set_error('anotacion_limit_per_node', t('Please enter number.'));
  }
}



