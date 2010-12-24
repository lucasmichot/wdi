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

  return $form;
}

/**
 * List available tasks
 *
 * @see system_settings_form()
 */
function wdi_admin_settings_tasks_list() {

  drupal_set_title(t('Available tasks'));

  $result = db_select('wdi_tasks', 'wt')->fields('wt', array('tid', 'name', 'action_uri', 'icon_uri', 'weight'))->execute();

  $rows = array();
  foreach ($result as $record) {

    $row = array(
      $record->weight,
      $record->name,
      l($record->action_uri, $record->action_uri),
      theme('image', array('path' => $record->icon_uri, 'alt' => t('Icon for task @name', array('@name' => $record->name)))),
      l('Delete', 'admin/config/search/wdi/tasks/delete/' . $record->tid) . ' / ' . l('Edit', 'admin/config/search/wdi/tasks/edit/' . $record->tid),
    );

    $rows[] = $row;
  }

  $header = array(
    t('Weight'),
    t('Name'),
    t('Action URI'),
    t('Icon'),
    t('Actions'),
  );

  $variables = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(),
    'colgroups' => NULL,
    'caption' => NULL,
    'sticky' => NULL,
    'empty' => t('You have not created any task yet. Create one now.'),
  );

  $form = array();
  
  $form['tasks'] = array(
    '#markup' => theme_table($variables),
  );

  return $form;
}

/**
 * Add a task
 *
 * @see system_settings_form()
 */
function wdi_admin_settings_tasks_add() {

  drupal_set_title(t('Add a new task'));

  $form = array();

  $form['task_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Task name'),
    '#description' => t('The name of the task.'),
    '#required' => TRUE,
    '#default_value' => NULL,
  );

  $form['task_action_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Action URI'),
    '#description' => t('The action URI of the task. This must a valid internal or external URL.'),
    '#required' => TRUE,
    '#default_value' => NULL,
  );

  $form['task_icon_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check here if you want the default website icon to be used for this task.'),
    '#default_value' => TRUE,
  );


  $form['task_icon'] = array(
    '#type' => 'fieldset',
    '#title' => t('Icon'),
    '#default_value' => NULL,
    '#description' => t('The path to the image file you would like to use as your custom shortcut icon.'),
    '#states' => array(
      'visible' => array(
        ':input[name="task_icon_default"]' => array('checked' => FALSE),
      ),
    ),
  );


  $form['task_icon']['task_icon_uri_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to custom icon'),
    '#default_value' => NULL,
    '#description' => t('The path to the image file you would like to use as your custom shortcut icon.'),
  );

  $validators = array('file_validate_extensions' => array('ico png gif jpg jpeg apng svg'));


  $form['task_icon']['task_icon_uri_path_upload'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload icon image'),
    '#description' => t("If you don't have direct file access to the server, use this field to upload your shortcut icon. Only ico and png files are allowed."),
    '#default_value' => NULL,
    '#upload_validators' => array(
      'file_validate_extensions' => array('ico png'),
    ),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function wdi_admin_settings_tasks_add_validate($form, &$form_state) {}

function wdi_admin_settings_tasks_add_submit($form, &$form_state) {}

function wdi_admin_settings_tasks_add_icon_validate() {
  return TRUE;
}

