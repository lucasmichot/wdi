<?php
// $Id$

/**
 * @file
 * Administrative page callbacks for the wdi module.
 */

/**
 * Administration settings form.
 *
 * @see system_settings_form()
 */
function wdi_admin_settings() {

  // Set window title
  drupal_set_title(t('Windows Desktop Integration'));

  $form = array();

  $form['wdi_application_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Application name'),
    '#description' => t('The name of the shortcut. If missing, the document title is used instead.'),
    '#default_value' => variable_get('wdi_application_name', variable_get('site_name')),
  );

  $form['wdi_msapplication_tooltip'] = array(
    '#type' => 'textfield',
    '#title' => t('Application tooltip'),
    '#description' => t('Optional text that is displayed as a tooltip when the mouse pointer hovers over the pinned site shortcut icon in the Windows Start menu or desktop.'),
    '#default_value' => variable_get('wdi_msapplication_tooltip', variable_get('site_slogan')),
  );

  $form['wdi_msapplication_starturl'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#title' => t('Application start URL'),
    '#description' => t('The root URL of the application. If missing, the address of the current page is used instead.'),
    '#default_value' => variable_get('wdi_msapplication_starturl', variable_get('site_frontpage')),
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? NULL : '?q='),
  );

  $form['wdi_msapplication_navbutton_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Navigation buttons color'),
    '#description' => t('The color of the Back and Forward buttons in the pinned site browser window. Any named color, or hex color value as defined by Cascading Style Sheets (CSS), Level 3 (CSS3) is valid. If this meta element is absent, the color is based on the shortcut icon.'),
    '#default_value' => variable_get('wdi_msapplication_navbutton_color'),
  );

  $form['wdi_msapplication_window_width'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#maxlength' => 4,
    '#title' => t('Window width'),
    '#description' => t('The initial width of the pinned site browser window. Minimum is 800. Note that user action overwrites this value.'),
    '#default_value' => variable_get('wdi_msapplication_window_width', 800),
    '#field_suffix' => t('px'),
  );

  $form['wdi_msapplication_window_height'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#maxlength' => 4,
    '#title' => t('Window height'),
    '#description' => t('The initial height of the pinned site browser window. Minimum is 600. Note that user action overwrites this value.'),
    '#default_value' => variable_get('wdi_msapplication_window_height', 600),
    '#field_suffix' => t('px'),
  );

  $form = system_settings_form($form);

  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#submit' => array('wdi_admin_settings_reset_submit'),
  );

  // Return the form
  return $form;
}

/**
 * List available tasks
 *
 * @see system_settings_form()
 */
function wdi_tasks_overview() {
  // Get all tasks
  $tasks = wdi_tasks();

  // Declare form
  $form = array();

  // The form must be dispalyed as a tree
  $form['#tree'] = TRUE;

  // For each task
  if (count($tasks)) {
    foreach ($tasks as $tid => $task) {
      $form['tasks'][$tid]['weight'] = array(
        '#type' => 'weight',
        '#title' => t('Weight for @title', array('@title' => $task->name)),
        '#title_display' => 'invisible',
        '#default_value' => $task->weight,
        '#attributes' => array('class' => array('wdi-tasks-order-weight')),
      );

      $image_properties = array(
        'path' => $task->icon_uri,
        'height' => 16,
        'width' => 16,
        'alt' => t('Icon for task : @name', array('@name' => $task->name)),
      );
      $form['tasks'][$tid]['icon_uri'] = array(
        '#markup' => theme('image', $image_properties),
      );

      $form['tasks'][$tid]['name'] = array(
        '#markup' => $task->name,
      );

      $form['tasks'][$tid]['action_uri'] = array(
        '#type' => 'link',
        '#title' => $task->action_uri,
        '#href' => $task->action_uri,
      );

      $form['tasks'][$tid]['edit'] = array(
        '#type' => 'link',
        '#title' => t('edit'),
        '#href' => 'admin/config/search/wdi/tasks/edit/' . $tid,
      );

      $form['tasks'][$tid]['delete'] = array(
        '#type' => 'link',
        '#title' => t('delete'),
        '#href' => 'admin/config/search/wdi/tasks/delete/' . $tid,
      );
    }
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
    '#disabled' => count($tasks) < 2,
  );

  return $form;
}

/**
 * Add a task
 *
 * @see system_settings_form()
 */
function wdi_admin_settings_tasks_add() {

  // Set window title
  drupal_set_title(t('Add a new task'));

  // Create the form
  $form = array();

  $form['task_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Task name'),
    '#description' => t('The name of the task.'),
    '#required' => TRUE,
    '#default_value' => NULL,
  );

  $form['task_action_uri_options'] = array(
    '#type' => 'radios',
    '#title' => t('Action URI type'),
    '#description' => t('Select wether the Action URI is relative or absolute.'),
    '#default_value' => 'relative',
    '#options' => array(
      'relative' => t('Use a relative URL'),
      'absolute' => t('Use an absolute URL'),
    ),
  );

  $form['task_action_uri_relative'] = array(
    '#type' => 'textfield',
    '#title' => t('Action URI'),
    '#description' => t('The relative action URI of the task. This must a valid internal URL.'),
    '#default_value' => NULL,
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? NULL : '?q='),
    '#states' => array(
      'visible' => array(
        ':input[name="task_action_uri_options"]' => array('value' => 'relative'),
      ),
    ),
  );

  $form['task_action_uri_absolute'] = array(
    '#type' => 'textfield',
    '#title' => t('Action URI'),
    '#description' => t('The absolute action URI of the task. This must a valid external URL.'),
    '#default_value' => NULL,
    '#states' => array(
      'visible' => array(
        ':input[name="task_action_uri_options"]' => array('value' => 'absolute'),
      ),
    ),
  );

  $form['task_icon_options'] = array(
    '#type' => 'radios',
    '#title' => t('Icon option'),
    '#default_value' => 'default',
    '#options' => array(
      'default' => t('Use default website icon'),
      'custom' => t('Use a path to a custom icon'),
      'upload' => t('Upload an icon image'),
    ),
  );

  $form['task_icon_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to custom icon'),
    '#default_value' => NULL,
    '#description' => t('The path to the image file you would like to use as your custom shortcut icon.'),
    '#states' => array(
      'visible' => array(
        ':input[name="task_icon_options"]' => array('value' => 'custom'),
      ),
    ),
  );

  $form['task_icon_upload_container'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="task_icon_options"]' => array('value' => 'upload'),
      ),
    ),
  );

  $form['task_icon_upload_container']['task_icon_upload'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload icon image'),
    '#description' => t("If you don't have direct file access to the server, use this field to upload your shortcut icon. Only ico and png files are allowed."),
    '#default_value' => NULL,
    '#upload_validators' => array(
      'file_validate_extensions' => array('ico png'),
    ),
    '#upload_location' => 'public://wdi',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#validate' => array('wdi_admin_settings_tasks_add_validate'),
    '#submit' => array('wdi_admin_settings_tasks_add_submit'),
  );

  // Return the form
  return $form;
}

function wdi_admin_settings_tasks_add_validate(&$form, &$form_state) {
  // Trim all values
  array_map('trim', $form_state['values']);

  // Get values
  $values = $form_state['values'];

  // Check task name
  if (empty($values['task_name'])) {
    form_set_error('task_name', t('Task name is required.'));
  }

  // Check Action URI
  if ($values['task_action_uri_options'] == 'relative') {
    if (empty($values['task_action_uri_relative'])) {
      form_set_error('task_action_uri_relative', t('Action URI is required.'));
    }
  }
  elseif ($values['task_action_uri_options'] == 'absolute') {
    if (empty($values['task_action_uri_absolute'])) {
      form_set_error('task_action_uri_absolute', t('Action URI is required.'));
    }
    elseif (valid_url($values['task_action_uri_absolute']) == FALSE) {
      form_set_error('task_action_uri_absolute', t('Action URI is not a valid URL.'));
    }
  }

  // Check icon URI
  if ($values['task_icon_options'] == 'custom') {
    if (empty($values['task_icon_custom'])) {
      form_set_error('task_icon_custom', t('Path to custom icon is required.'));
    }
    elseif (valid_url($values['task_icon_custom']) == FALSE || preg_match('/^.*\.(ico|png)$/i', $values['task_icon_custom']) == FALSE) {
      form_set_error('task_icon_custom', t('This is not a valid icon image.'));
    }
  }
  elseif ($values['task_icon_options'] == 'upload' && empty($values['task_icon_upload'])) {
    form_set_error('task_icon_upload', t('You must upload a custom icon.'));
  }
}

function wdi_admin_settings_tasks_add_submit(&$form, &$form_state) {
  dpm($form_state);


  $record         = new stdClass();
  $record->weight = time();
  $record->name   = $form_state['values']['task_name'];

  switch ($form_state['values']['task_action_uri_options']) {
    case 'relative':
      $record->action_uri_type = 0;
      $record->action_uri = $form_state['values']['task_action_uri_relative'];
      break;

    case 'absolute':
      $record->action_uri_type = 1;
      $record->action_uri = $form_state['values']['task_action_uri_absolute'];
      break;
  }

  if ($fid = $form_state['values']['task_icon_upload']) {
    $file = file_load($fid);
    $file_uploaded = TRUE;
  }
  else {
    $file_uploaded = FALSE;
  }

  switch ($form_state['values']['task_icon_options']) {
    case 'default':
      if ($file_uploaded) {
        file_delete($file, TRUE);
      }
      $record->icon_uri = -1;
      break;

    case 'custom':
      if ($file_uploaded) {
        file_delete($file, TRUE);
      }
      $record->icon_uri = $form_state['values']['task_icon_custom'];
      break;

    case 'upload':
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      $record->icon_uri = $fid;
      break;
  }

  // Write task to the database
  drupal_write_record('wdi_tasks', $record);

  // Clear the cache
  cache_clear_all('wdi_tasks', 'cache');

  // Inform the user
  drupal_set_message(t('Task added'));
}

