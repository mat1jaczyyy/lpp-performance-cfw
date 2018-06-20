# uncompyle6 version 3.2.3
# Python bytecode 2.7 (62211)
# Decompiled from: Python 3.5.4 (v3.5.4:3f56838976, Aug  7 2017, 12:56:33) 
# [GCC 4.2.1 (Apple Inc. build 5666) (dot 3)]
# Embedded file name: /Users/versonator/Jenkins/live/output/mac_64_static/Release/python-bundle/MIDI Remote Scripts/Launchpad_Pro/Launchpad_Pro.py
# Compiled at: 2018-04-23 20:27:04
from __future__ import absolute_import, print_function, unicode_literals
from functools import partial
import Live
from _Framework.Util import const
from _Framework.Dependency import inject
from _Framework.SubjectSlot import subject_slot
from _Framework.Layer import Layer
from _Framework.ControlSurface import OptimizedControlSurface
from _Framework.IdentifiableControlSurface import IdentifiableControlSurface
from _Framework.ModesComponent import ModesComponent, LayerMode, AddLayerMode, ReenterBehaviour
from _Framework.InputControlElement import MIDI_NOTE_TYPE, MIDI_CC_TYPE
from _Framework.ComboElement import ComboElement
from _Framework.ButtonMatrixElement import ButtonMatrixElement
from .Colors import LIVE_COLORS_TO_MIDI_VALUES, RGB_COLOR_TABLE
from .SkinDefault import make_default_skin
from .SpecialMidiMap import SpecialMidiMap, make_button, make_multi_button, make_slider
from .BackgroundComponent import ModifierBackgroundComponent, BackgroundComponent
from .ActionsComponent import ActionsComponent
from .ClipActionsComponent import ClipActionsComponent
from .LedLightingComponent import LedLightingComponent
from .TranslationComponent import TranslationComponent
from .TargetTrackComponent import TargetTrackComponent
from .SpecialDeviceComponent import SpecialDeviceComponent
from .DeviceNavigationComponent import DeviceNavigationComponent
from .SpecialSessionRecordingComponent import SpecialSessionRecordingComponent
from .DrumGroupFinderComponent import DrumGroupFinderComponent
from .DrumGroupComponent import DrumGroupComponent
from .SpecialMixerComponent import SpecialMixerComponent
from .SpecialSessionComponent import SpecialSessionComponent as SessionComponent, SpecialClipSlotComponent, SpecialSessionZoomingComponent as SessionZoomingComponent, SessionZoomingManagerComponent
from .SpecialModesComponent import SpecialModesComponent, SpecialReenterBehaviour, CancelingReenterBehaviour
from .UserMatrixComponent import UserMatrixComponent
from . import consts
NUM_TRACKS = 8
NUM_SCENES = 8

class MidiMap(SpecialMidiMap):

    def __init__(self, *a, **k):
        super(MidiMap, self).__init__(*a, **k)
        left_button_names = ('Session_Record_Button', 'Double_Loop_Button', 'Duplicate_Button',
                             'Quantize_Button', 'Delete_Button', 'Undo_Button', 'Click_Button',
                             'Shift_Button')
        default_states = {True: b'DefaultButton.On', 
           False: b'DefaultButton.Off'}
        rec_states = {True: b'Recording.On', 
           False: b'Recording.Off'}
        shift_states = {True: b'Misc.ShiftOn', 
           False: b'Misc.Shift'}
        for index, val in enumerate(left_button_names):
            if val in ('Session_Record_Button', 'Undo_Button', 'Click_Button'):
                self.add_button(val, 0, (index + 1) * 10, MIDI_CC_TYPE, default_states=rec_states if val == b'Session_Record_Button' else default_states)
            else:
                self.add_modifier_button(val, 0, (index + 1) * 10, MIDI_CC_TYPE, default_states=shift_states if val == b'Shift_Button' else default_states)

        self.add_button(b'Record_Arm_Mode_Button', 0, 1, MIDI_CC_TYPE, default_states={True: b'Mode.RecordArm.On', 
           False: b'Mode.RecordArm.Off'})
        self.add_button(b'Track_Select_Mode_Button', 0, 2, MIDI_CC_TYPE, default_states={True: b'Mode.TrackSelect.On', 
           False: b'Mode.TrackSelect.Off'})
        self.add_button(b'Mute_Mode_Button', 0, 3, MIDI_CC_TYPE, default_states={True: b'Mode.Mute.On', 
           False: b'Mode.Mute.Off'})
        self.add_button(b'Solo_Mode_Button', 0, 4, MIDI_CC_TYPE, default_states={True: b'Mode.Solo.On', 
           False: b'Mode.Solo.Off'})
        self.add_button(b'Volume_Mode_Button', 0, 5, MIDI_CC_TYPE, default_states={True: b'Mode.Volume.On', 
           False: b'Mode.Volume.Off'})
        self.add_button(b'Pan_Mode_Button', 0, 6, MIDI_CC_TYPE, default_states={True: b'Mode.Pan.On', 
           False: b'Mode.Pan.Off'})
        self.add_button(b'Sends_Mode_Button', 0, 7, MIDI_CC_TYPE, default_states={True: b'Mode.Sends.On', 
           False: b'Mode.Sends.Off'})
        self.add_button(b'Stop_Clip_Mode_Button', 0, 8, MIDI_CC_TYPE, default_states={True: b'Mode.StopClip.On', 
           False: b'Mode.StopClip.Off'})
        self._arrow_button_names = [
         b'Arrow_Up_Button',
         b'Arrow_Down_Button',
         b'Arrow_Left_Button',
         b'Arrow_Right_Button']
        arrow_button_states = {b'Pressed': b'DefaultButton.On', 
           b'Enabled': b'DefaultButton.Off', 
           True: b'DefaultButton.On', 
           False: b'DefaultButton.Disabled'}
        for index, val in enumerate(self._arrow_button_names):
            self.add_button(val, 0, index + 91, MIDI_CC_TYPE, default_states=arrow_button_states)

        self.add_modifier_button(b'Session_Mode_Button', 0, 95, MIDI_CC_TYPE, default_states={True: b'Mode.Session.On', 
           False: b'Mode.Session.Off'}, element_factory=make_multi_button)
        self.add_button(b'Note_Mode_Button', 0, 96, MIDI_CC_TYPE, element_factory=make_multi_button)
        self.add_button(b'Device_Mode_Button', 0, 97, MIDI_CC_TYPE, default_states={True: b'Mode.Device.On', 
           False: b'Mode.Device.Off'}, element_factory=make_multi_button)
        self.add_button(b'User_Mode_Button', 0, 98, MIDI_CC_TYPE, default_states={True: b'Mode.User.On', 
           False: b'Mode.User.Off'}, element_factory=make_multi_button, color_slaves=True)
        self.add_matrix(b'Scene_Launch_Button_Matrix', make_button, 0, [[ identifier for identifier in xrange(89, 18, -10) ]], MIDI_CC_TYPE)
        self[b'Scene_Stop_Button_Matrix'] = self[b'Scene_Launch_Button_Matrix'].submatrix[:7, :]
        self[b'Scene_Stop_Button_Matrix'].name = b'Scene_Stop_Button_Matrix'
        self[b'Stop_All_Clips_Button'] = self[b'Scene_Launch_Button_Matrix_Raw'][0][7]
        self.add_matrix(b'Main_Button_Matrix', make_button, 0, [ [ identifier for identifier in xrange(start, start + NUM_TRACKS) ] for start in xrange(81, 10, -10)
                                                               ], MIDI_NOTE_TYPE)
        self[b'Mixer_Button_Matrix'] = self[b'Main_Button_Matrix'].submatrix[:, 7:]
        self[b'Mixer_Button_Matrix'].name = b'Mixer_Button_Matrix'
        matrix_rows_with_session_button_raw = [ [ self.with_session_button(self[b'Main_Button_Matrix_Raw'][row][column]) for column in xrange(8) ] for row in xrange(8)
                                              ]
        self[b'Main_Button_Matrix_With_Session_Button'] = ButtonMatrixElement(rows=matrix_rows_with_session_button_raw, name=b'Main_Button_Matrix_With_Session_Button')
        note_buttons_raw = []
        for identifier in xrange(128):
            if identifier not in self[b'Main_Button_Matrix_Ids']:
                button = make_button(b'Note_Button_' + str(identifier), 0, identifier, MIDI_NOTE_TYPE)
                button.set_enabled(False)
                button.set_channel(consts.CHROM_MAP_CHANNEL)
                note_buttons_raw.append(button)

        self[b'Note_Button_Matrix'] = ButtonMatrixElement(rows=[
         note_buttons_raw], name=b'Note_Button_Matrix')

        def make_raw_drum_matrix():
            result = []
            for row in xrange(7, -1, -1):
                button_row = []
                row_offset = 8 + (7 - row) * 4
                for column in xrange(8):
                    column_offset = 28 if column >= 4 else 0
                    identifier = row * 8 + column + row_offset + column_offset
                    matrix_coords = self[b'Main_Button_Matrix_Ids'].get(identifier)
                    if matrix_coords:
                        button_row.append(self[b'Main_Button_Matrix_Raw'][matrix_coords[1]][matrix_coords[0]])
                    else:
                        button_row.append(make_button(b'Drum_Note_Button_' + str(identifier), 0, identifier, MIDI_NOTE_TYPE))

                result.append(button_row)

            return result

        self[b'Drum_Button_Matrix'] = ButtonMatrixElement(rows=make_raw_drum_matrix(), name=b'Drum_Button_Matrix')
        self.add_matrix(b'Slider_Button_Matrix', make_slider, 0, [[ identifier for identifier in xrange(21, 29) ]], MIDI_CC_TYPE)
        for index, slider in enumerate(self[b'Slider_Button_Matrix_Raw'][0]):
            slider.set_index(index)

        self.create_user_mode_controls()

    def create_user_mode_controls(self):
        """
        Creates control elements that aren't used in the script
        but need to exist so they can be grabbed and observed
        via Max for Live.
        """
        for channel in consts.USER_MODE_CHANNELS:
            channel_name = channel + 1
            self.add_matrix(b'User_Button_Matrix_Ch_%d' % (channel_name,), make_button, channel, consts.USER_MATRIX_IDENTIFIERS, MIDI_NOTE_TYPE)
            self.add_matrix(b'User_Left_Side_Button_Matrix_Ch_%d' % (channel_name,), make_button, channel, [ [identifier] for identifier in xrange(108, 116) ], MIDI_NOTE_TYPE)
            self.add_matrix(b'User_Right_Side_Button_Matrix_Ch_%d' % (channel_name,), make_button, channel, [ [identifier] for identifier in xrange(100, 108) ], MIDI_NOTE_TYPE)
            self.add_matrix(b'User_Bottom_Button_Matrix_Ch_%d' % (channel_name,), make_button, channel, [[ identifier for identifier in xrange(116, 124) ]], MIDI_NOTE_TYPE)
            self.add_matrix(b'User_Arrow_Button_Matrix_Ch_%d' % (channel_name,), make_button, channel, [[ identifier for identifier in xrange(91, 95) ]], MIDI_CC_TYPE)

    def with_shift(self, button_name):
        return ComboElement(self[button_name], modifiers=[
         self[b'Shift_Button']], name=b'Shifted_' + button_name)

    def with_session_button(self, button):
        return ComboElement(button, modifiers=[
         self[b'Session_Mode_Button']], name=button.name + b'_With_Session_Button')


class Launchpad_Pro(IdentifiableControlSurface, OptimizedControlSurface):
    identity_request = consts.SYSEX_IDENTITY_REQUEST

    def __init__(self, c_instance, *a, **k):
        product_id_bytes = consts.MANUFACTURER_ID + consts.DEVICE_CODE
        super(Launchpad_Pro, self).__init__(c_instance=c_instance, product_id_bytes=product_id_bytes, *a, **k)
        self._challenge = Live.Application.get_random_int(0, 400000000) & 2139062143
        with self.component_guard():
            self._skin = make_default_skin()
            with inject(skin=const(self._skin)).everywhere():
                self._midimap = MidiMap()
            self._target_track_component = TargetTrackComponent(name=b'Target_Track')
            self._create_background()
            self._create_global_component()
            self._last_sent_mode_byte = None
            with inject(layout_setup=const(self._layout_setup), should_arm=const(self._should_arm_track)).everywhere():
                self._create_session()
                self._create_recording()
                self._create_actions()
                self._create_drums()
                self._create_mixer()
                self._create_device()
                self._create_modes()
                self._create_user()
            self._on_session_record_changed.subject = self.song()
        self.set_device_component(self._device)
        self._on_session_record_changed()
        return

    def disconnect(self):
        self._send_midi(consts.TURN_OFF_LEDS)
        self._send_midi(consts.QUIT_MESSAGE)
        super(Launchpad_Pro, self).disconnect()

    def _create_background(self):
        self._modifier_background_component = ModifierBackgroundComponent(name=b'Background_Component', is_enabled=False, layer=Layer(shift_button=self._midimap[b'Shift_Button']))
        self._shifted_background = BackgroundComponent(name=b'No_Op_Shifted_Buttons', is_enabled=False, layer=Layer(click_bitton=self._midimap.with_shift(b'Click_Button'), delete_button=self._midimap.with_shift(b'Delete_Button'), duplicate_button=self._midimap.with_shift(b'Duplicate_Button'), double_button=self._midimap.with_shift(b'Double_Loop_Button'), session_record_button=self._midimap.with_shift(b'Session_Record_Button')))

    def _create_global_component(self):
        self._actions_component = ActionsComponent(name=b'Global_Actions', is_enabled=False, layer=Layer(undo_button=self._midimap[b'Undo_Button'], redo_button=self._midimap.with_shift(b'Undo_Button'), metronome_button=self._midimap[b'Click_Button'], quantization_on_button=self._midimap.with_shift(b'Quantize_Button')))

    def _create_session(self):
        self._session = SessionComponent(NUM_TRACKS, NUM_SCENES, auto_name=True, is_enabled=False, enable_skinning=True, layer=Layer(track_bank_left_button=self._midimap[b'Arrow_Left_Button'], track_bank_right_button=self._midimap[b'Arrow_Right_Button'], scene_bank_up_button=self._midimap[b'Arrow_Up_Button'], scene_bank_down_button=self._midimap[b'Arrow_Down_Button']))
        self._session.set_enabled(True)
        self._session.set_rgb_mode(LIVE_COLORS_TO_MIDI_VALUES, RGB_COLOR_TABLE)
        SpecialClipSlotComponent.quantization_component = self._actions_component
        for scene_index in xrange(NUM_SCENES):
            scene = self._session.scene(scene_index)
            scene.layer = Layer(select_button=self._midimap[b'Shift_Button'], delete_button=self._midimap[b'Delete_Button'], duplicate_button=self._midimap[b'Duplicate_Button'])
            for track_index in xrange(NUM_TRACKS):
                slot = scene.clip_slot(track_index)
                slot.layer = Layer(select_button=self._midimap[b'Shift_Button'], delete_button=self._midimap[b'Delete_Button'], duplicate_button=self._midimap[b'Duplicate_Button'], double_loop_button=self._midimap[b'Double_Loop_Button'], quantize_button=self._midimap[b'Quantize_Button'])

        self._session_zoom = SessionZoomingComponent(self._session, name=b'Session_Overview', is_enabled=True, enable_skinning=True)

    def _create_recording(self):
        self._session_record = SpecialSessionRecordingComponent(self._target_track_component, name=b'Session_Recording', is_enabled=False, layer=Layer(record_button=self._midimap[b'Session_Record_Button']))

    def _create_actions(self):
        self._clip_actions_component = ClipActionsComponent(self._target_track_component, name=b'Clip_Actions', is_enabled=False, layer=Layer(duplicate_button=self._midimap[b'Duplicate_Button'], double_button=self._midimap[b'Double_Loop_Button'], quantize_button=self._midimap[b'Quantize_Button']))
        ClipActionsComponent.quantization_component = self._actions_component

    def _create_drums(self):
        self._drum_group_finder = DrumGroupFinderComponent(self._target_track_component, name=b'Drum_Group_Finder', is_enabled=False, layer=None)
        self._on_drum_group_changed.subject = self._drum_group_finder
        self._drum_group_finder.set_enabled(True)
        self._drum_group = DrumGroupComponent(self._clip_actions_component, name=b'Drum_Group_Control', translation_channel=consts.DR_MAP_CHANNEL)
        self._drum_group.set_enabled(True)
        return

    def _create_mixer(self):
        self._mixer = SpecialMixerComponent(NUM_TRACKS, auto_name=True, is_enabled=True, invert_mute_feedback=True)
        self._mixer.name = b'Mixer_Control'
        self._session.set_mixer(self._mixer)

    def _create_device(self):
        self._device = SpecialDeviceComponent(name=b'Device_Control', is_enabled=False, device_selection_follows_track_selection=True)
        self._device_navigation = DeviceNavigationComponent(name=b'Device_Navigation')
        self._device_background = BackgroundComponent(name=b'Device_Background_Component')

    def _setup_drum_group(self):
        self._drum_group.set_drum_group_device(self._drum_group_finder.drum_group)

    def _create_translation(self, comp_name, channel, button_layer, should_enable=True, should_reset=True):
        translation_component = TranslationComponent(name=comp_name, translated_channel=channel, should_enable=should_enable, should_reset=should_reset, is_enabled=False, layer=button_layer)
        setattr(self, b'_' + comp_name.lower(), translation_component)
        return translation_component

    def _create_modes(self):
        self._modes = ModesComponent(name=b'Launchpad_Modes', is_enabled=False)
        self._session_layer_mode = AddLayerMode(self._session, Layer(scene_launch_buttons=self._midimap[b'Scene_Launch_Button_Matrix'], clip_launch_buttons=self._midimap[b'Main_Button_Matrix'], delete_button=self._midimap[b'Delete_Button'], duplicate_button=self._midimap[b'Duplicate_Button'], double_button=self._midimap[b'Double_Loop_Button'], quantize_button=self._midimap[b'Quantize_Button']))
        action_button_background = BackgroundComponent(name=b'No_Op_Buttons')
        self._action_button_background_layer_mode = LayerMode(action_button_background, Layer(delete_button=self._midimap[b'Delete_Button'], quantize_button=self._midimap[b'Quantize_Button'], duplicate_button=self._midimap[b'Duplicate_Button'], double_button=self._midimap[b'Double_Loop_Button']))
        self._clip_delete_layer_mode = AddLayerMode(self._clip_actions_component, layer=Layer(delete_button=self._midimap[b'Delete_Button']))
        self._create_session_zooming_modes()
        self._create_session_mode()
        self._create_note_modes()
        self._create_device_mode()
        self._create_user_mode()
        self._create_record_arm_mode()
        self._create_track_select_mode()
        self._create_mute_mode()
        self._create_solo_mode()
        self._create_volume_mode()
        self._create_pan_mode()
        self._create_sends_mode()
        self._create_stop_clips_mode()
        self._modes.layer = Layer(session_mode_button=self._midimap[b'Session_Mode_Button'], note_mode_button=self._midimap[b'Note_Mode_Button'], device_mode_button=self._midimap[b'Device_Mode_Button'], user_mode_button=self._midimap[b'User_Mode_Button'], record_arm_mode_button=self._midimap[b'Record_Arm_Mode_Button'], track_select_mode_button=self._midimap[b'Track_Select_Mode_Button'], mute_mode_button=self._midimap[b'Mute_Mode_Button'], solo_mode_button=self._midimap[b'Solo_Mode_Button'], volume_mode_button=self._midimap[b'Volume_Mode_Button'], pan_mode_button=self._midimap[b'Pan_Mode_Button'], sends_mode_button=self._midimap[b'Sends_Mode_Button'], stop_clip_mode_button=self._midimap[b'Stop_Clip_Mode_Button'])
        self._modes.selected_mode = b'session_mode'
        self._on_layout_changed.subject = self._modes

    def _create_session_zooming_modes(self):
        session_zoom_layer = Layer(button_matrix=self._midimap[b'Main_Button_Matrix'], nav_left_button=self._midimap[b'Arrow_Left_Button'], nav_right_button=self._midimap[b'Arrow_Right_Button'], nav_up_button=self._midimap[b'Arrow_Up_Button'], nav_down_button=self._midimap[b'Arrow_Down_Button'])
        session_zooming_layer_mode = LayerMode(self._session_zoom, session_zoom_layer)
        self._session_zooming_manager = SessionZoomingManagerComponent(self._modes, is_enabled=False)
        session_zooming_button_layer_mode = LayerMode(self._session_zooming_manager, Layer(session_zooming_button=self._midimap[b'Session_Mode_Button']))
        self._prioritized_session_zooming_button_layer_mode = LayerMode(self._session_zooming_manager, Layer(session_zooming_button=self._midimap[b'Session_Mode_Button'], priority=1))
        self._session_zooming_background = BackgroundComponent(name=b'Session_Zooming_Background')
        session_zooming_background_layer_mode = LayerMode(self._session_zooming_background, Layer(scene_launch_buttons=self._midimap[b'Scene_Launch_Button_Matrix'], delete_button=self._midimap[b'Delete_Button'], quantize_button=self._midimap[b'Quantize_Button'], duplicate_button=self._midimap[b'Duplicate_Button'], double_loop_button=self._midimap[b'Double_Loop_Button']))
        self._modes.add_mode(b'session_zooming_mode', [
         self._session_zooming_manager,
         session_zooming_button_layer_mode,
         session_zooming_layer_mode,
         session_zooming_background_layer_mode])
        self._modes.add_mode(b'prioritized_session_zooming_mode', [
         partial(self._layout_switch, consts.SESSION_LAYOUT_SYSEX_BYTE),
         self._session_zooming_manager,
         self._prioritized_session_zooming_button_layer_mode,
         session_zooming_layer_mode,
         session_zooming_background_layer_mode,
         self.update])

    def _create_session_mode(self):
        self._modes.add_mode(b'session_mode', [
         partial(self._layout_setup, consts.SESSION_LAYOUT_SYSEX_BYTE),
         self._session_layer_mode,
         self._session.update_navigation_buttons], behaviour=CancelingReenterBehaviour(b'session_zooming_mode'))

    def _create_note_modes(self):
        note_mode_matrix_translation = self._create_translation(b'Note_Mode_Matrix_Translation', consts.CHROM_MAP_CHANNEL, Layer(button_matrix=self._midimap[b'Main_Button_Matrix'], note_button_matrix=self._midimap[b'Note_Button_Matrix'], drum_matrix=self._midimap[b'Drum_Button_Matrix'], mixer_button_matrix=self._midimap[b'Mixer_Button_Matrix']), should_enable=False)
        note_mode_scene_launch_translation = self._create_translation(b'Note_Mode_Scene_Launch_Translation', consts.CHROM_MAP_CHANNEL, Layer(scene_launch_buttons=self._midimap[b'Scene_Launch_Button_Matrix']))
        scale_setup_mode_button_lighting = LedLightingComponent(name=b'LED_Lighting_Component', is_enabled=False, layer=Layer(button=self._midimap.with_shift(b'Note_Mode_Button')))
        drum_mode_note_matrix_translation = self._create_translation(b'Drum_Mode_Note_Button_Translation', 0, Layer(note_button_matrix=self._midimap[b'Note_Button_Matrix']), should_enable=False, should_reset=False)
        drum_group_layer_mode = LayerMode(self._drum_group, layer=Layer(scroll_up_button=self._midimap[b'Arrow_Left_Button'], scroll_down_button=self._midimap[b'Arrow_Right_Button'], scroll_page_up_button=self._midimap[b'Arrow_Up_Button'], scroll_page_down_button=self._midimap[b'Arrow_Down_Button'], drum_matrix=self._midimap[b'Drum_Button_Matrix'], select_button=self._midimap[b'Shift_Button'], delete_button=self._midimap[b'Delete_Button']))
        self._note_modes = SpecialModesComponent(name=b'Note_Modes')
        self._note_modes.add_mode(b'chromatic_mode', [
         partial(self._layout_setup, consts.NOTE_LAYOUT_SYSEX_BYTE),
         self._clip_delete_layer_mode,
         note_mode_matrix_translation,
         scale_setup_mode_button_lighting])
        self._note_modes.add_mode(b'drum_mode', [
         partial(self._layout_setup, consts.DRUM_LAYOUT_SYSEX_BYTE),
         self._setup_drum_group,
         drum_group_layer_mode,
         drum_mode_note_matrix_translation])
        self._note_modes.add_mode(b'audio_mode', [
         partial(self._layout_setup, consts.AUDIO_LAYOUT_SYSEX_BYTE),
         self._clip_delete_layer_mode])
        self._note_modes.set_enabled(False)
        self._modes.add_mode(b'note_mode', [
         note_mode_scene_launch_translation,
         self._note_modes,
         self._select_note_mode,
         self._select_target_track,
         self._clip_actions_component,
         self._show_playing_clip,
         self._set_clip_actions_type], behaviour=ReenterBehaviour(self.toggle_detail_view))
        self._session_record.set_modes_component(self._modes)
        self._session_record.set_note_mode_name(b'note_mode')

    def _create_device_mode(self):
        device_mode_scene_launch_translation = self._create_translation(b'Device_Mode_Scene_Launch_Translation', consts.DEVICE_MAP_CHANNEL, Layer(scene_launch_buttons=self._midimap[b'Scene_Launch_Button_Matrix']))
        device_layer_mode = LayerMode(self._device, layer=Layer(parameter_controls=self._midimap[b'Slider_Button_Matrix']))
        device_nav_layer_mode = LayerMode(self._device_navigation, layer=Layer(device_nav_left_button=self._midimap[b'Arrow_Left_Button'], device_nav_right_button=self._midimap[b'Arrow_Right_Button']))
        device_background_layer_mode = LayerMode(self._device_background, layer=Layer(arrow_up_button=self._midimap[b'Arrow_Up_Button'], arrow_down_button=self._midimap[b'Arrow_Down_Button']))
        self._modes.add_mode(b'device_mode', [
         partial(self._layout_setup, consts.FADER_LAYOUT_SYSEX_BYTE),
         self._device,
         device_layer_mode,
         device_nav_layer_mode,
         device_background_layer_mode,
         self._clip_actions_component,
         self._clip_delete_layer_mode,
         device_mode_scene_launch_translation,
         self._show_playing_clip,
         self._set_clip_actions_type], behaviour=ReenterBehaviour(self.toggle_detail_view))

    def _create_user_mode(self):
        self._modes.add_mode(b'user_mode', [
         partial(self._layout_setup, consts.USER_LAYOUT_SYSEX_BYTE)])

    def _create_record_arm_mode(self):
        arm_layer_mode = LayerMode(self._mixer, layer=Layer(arm_buttons=self._midimap[b'Mixer_Button_Matrix']))
        self._modes.add_mode(b'record_arm_mode', [
         partial(self._layout_setup, consts.SESSION_LAYOUT_SYSEX_BYTE),
         self._session_layer_mode,
         arm_layer_mode,
         self._session_zooming_manager,
         self._prioritized_session_zooming_button_layer_mode,
         self._session.update_navigation_buttons], behaviour=SpecialReenterBehaviour(b'session_mode'))

    def _create_track_select_mode(self):
        track_select_layer_mode = LayerMode(self._mixer, layer=Layer(track_select_buttons=self._midimap[b'Mixer_Button_Matrix']))
        self._modes.add_mode(b'track_select_mode', [
         partial(self._layout_setup, consts.SESSION_LAYOUT_SYSEX_BYTE),
         self._session_layer_mode,
         track_select_layer_mode,
         self._session_zooming_manager,
         self._prioritized_session_zooming_button_layer_mode,
         self._session.update_navigation_buttons], behaviour=SpecialReenterBehaviour(b'session_mode'))

    def _create_mute_mode(self):
        mute_layer_mode = LayerMode(self._mixer, layer=Layer(mute_buttons=self._midimap[b'Mixer_Button_Matrix']))
        self._modes.add_mode(b'mute_mode', [
         partial(self._layout_setup, consts.SESSION_LAYOUT_SYSEX_BYTE),
         self._session_layer_mode,
         mute_layer_mode,
         self._session_zooming_manager,
         self._prioritized_session_zooming_button_layer_mode,
         self._session.update_navigation_buttons], behaviour=SpecialReenterBehaviour(b'session_mode'))

    def _create_solo_mode(self):
        solo_layer_mode = LayerMode(self._mixer, layer=Layer(solo_buttons=self._midimap[b'Mixer_Button_Matrix']))
        self._modes.add_mode(b'solo_mode', [
         partial(self._layout_setup, consts.SESSION_LAYOUT_SYSEX_BYTE),
         self._session_layer_mode,
         solo_layer_mode,
         self._session_zooming_manager,
         self._prioritized_session_zooming_button_layer_mode,
         self._session.update_navigation_buttons], behaviour=SpecialReenterBehaviour(b'session_mode'))

    def _create_volume_mode(self):
        volume_mode_scene_launch_translation = self._create_translation(b'Volume_Mode_Scene_Launch_Translation', consts.VOLUME_MAP_CHANNEL, Layer(scene_launch_buttons=self._midimap[b'Scene_Launch_Button_Matrix']))
        volume_layer_mode = LayerMode(self._mixer, layer=Layer(volume_controls=self._midimap[b'Slider_Button_Matrix']))
        self._modes.add_mode(b'volume_mode', [
         partial(self._layout_setup, consts.FADER_LAYOUT_SYSEX_BYTE),
         volume_layer_mode,
         self._action_button_background_layer_mode,
         self._session_zooming_manager,
         self._prioritized_session_zooming_button_layer_mode,
         volume_mode_scene_launch_translation,
         self._session.update_navigation_buttons], behaviour=SpecialReenterBehaviour(b'session_mode'))

    def _create_pan_mode(self):
        pan_mode_scene_launch_translation = self._create_translation(b'Pan_Mode_Scene_Launch_Translation', consts.PAN_MAP_CHANNEL, Layer(scene_launch_buttons=self._midimap[b'Scene_Launch_Button_Matrix']))
        pan_layer_mode = LayerMode(self._mixer, layer=Layer(pan_controls=self._midimap[b'Slider_Button_Matrix']))
        self._modes.add_mode(b'pan_mode', [
         partial(self._layout_setup, consts.FADER_LAYOUT_SYSEX_BYTE),
         pan_layer_mode,
         self._action_button_background_layer_mode,
         self._session_zooming_manager,
         self._prioritized_session_zooming_button_layer_mode,
         pan_mode_scene_launch_translation,
         self._session.update_navigation_buttons], behaviour=SpecialReenterBehaviour(b'session_mode'))

    def _create_sends_mode(self):
        send_layer_mode = LayerMode(self._mixer, layer=Layer(send_controls=self._midimap[b'Slider_Button_Matrix'], send_select_buttons=self._midimap[b'Scene_Launch_Button_Matrix']))
        self._modes.add_mode(b'sends_mode', [
         partial(self._layout_setup, consts.FADER_LAYOUT_SYSEX_BYTE),
         send_layer_mode,
         self._action_button_background_layer_mode,
         self._session_zooming_manager,
         self._prioritized_session_zooming_button_layer_mode,
         self._session.update_navigation_buttons], behaviour=SpecialReenterBehaviour(b'session_mode'))

    def _create_stop_clips_mode(self):
        stop_layer_mode = AddLayerMode(self._session, Layer(stop_track_clip_buttons=self._midimap[b'Mixer_Button_Matrix'], stop_scene_clip_buttons=self._midimap[b'Scene_Stop_Button_Matrix'], stop_all_clips_button=self._midimap[b'Stop_All_Clips_Button']))
        self._modes.add_mode(b'stop_clip_mode', [
         partial(self._layout_setup, consts.SESSION_LAYOUT_SYSEX_BYTE),
         self._session_layer_mode,
         stop_layer_mode,
         self._session_zooming_manager,
         self._prioritized_session_zooming_button_layer_mode,
         self._session.update_navigation_buttons], behaviour=SpecialReenterBehaviour(b'session_mode'))

    def toggle_detail_view(self):
        view = self.application().view
        if view.is_view_visible(b'Detail'):
            if view.is_view_visible(b'Detail/DeviceChain'):
                view.show_view(b'Detail/Clip')
            else:
                view.show_view(b'Detail/DeviceChain')

    def _create_user(self):
        self._user_matrix_component = UserMatrixComponent(name=b'User_Matrix_Component', is_enabled=False, layer=Layer(user_button_matrix_ch_6=self._midimap[b'User_Button_Matrix_Ch_6'], user_button_matrix_ch_7=self._midimap[b'User_Button_Matrix_Ch_7'], user_button_matrix_ch_8=self._midimap[b'User_Button_Matrix_Ch_8'], user_button_matrix_ch_14=self._midimap[b'User_Button_Matrix_Ch_14'], user_button_matrix_ch_15=self._midimap[b'User_Button_Matrix_Ch_15'], user_button_matrix_ch_16=self._midimap[b'User_Button_Matrix_Ch_16'], user_left_side_button_matrix_ch_6=self._midimap[b'User_Left_Side_Button_Matrix_Ch_6'], user_left_side_button_matrix_ch_7=self._midimap[b'User_Left_Side_Button_Matrix_Ch_7'], user_left_side_button_matrix_ch_8=self._midimap[b'User_Left_Side_Button_Matrix_Ch_8'], user_left_side_button_matrix_ch_14=self._midimap[b'User_Left_Side_Button_Matrix_Ch_14'], user_left_side_button_matrix_ch_15=self._midimap[b'User_Left_Side_Button_Matrix_Ch_15'], user_left_side_button_matrix_ch_16=self._midimap[b'User_Left_Side_Button_Matrix_Ch_16'], user_right_side_button_matrix_ch_6=self._midimap[b'User_Right_Side_Button_Matrix_Ch_6'], user_right_side_button_matrix_ch_7=self._midimap[b'User_Right_Side_Button_Matrix_Ch_7'], user_right_side_button_matrix_ch_8=self._midimap[b'User_Right_Side_Button_Matrix_Ch_8'], user_right_side_button_matrix_ch_14=self._midimap[b'User_Right_Side_Button_Matrix_Ch_14'], user_right_side_button_matrix_ch_15=self._midimap[b'User_Right_Side_Button_Matrix_Ch_15'], user_right_side_button_matrix_ch_16=self._midimap[b'User_Right_Side_Button_Matrix_Ch_16'], user_bottom_button_matrix_ch_6=self._midimap[b'User_Bottom_Button_Matrix_Ch_6'], user_bottom_button_matrix_ch_7=self._midimap[b'User_Bottom_Button_Matrix_Ch_7'], user_bottom_button_matrix_ch_8=self._midimap[b'User_Bottom_Button_Matrix_Ch_8'], user_bottom_button_matrix_ch_14=self._midimap[b'User_Bottom_Button_Matrix_Ch_14'], user_bottom_button_matrix_ch_15=self._midimap[b'User_Bottom_Button_Matrix_Ch_15'], user_bottom_button_matrix_ch_16=self._midimap[b'User_Bottom_Button_Matrix_Ch_16'], user_arrow_button_matrix_ch_6=self._midimap[b'User_Arrow_Button_Matrix_Ch_6'], user_arrow_button_matrix_ch_7=self._midimap[b'User_Arrow_Button_Matrix_Ch_7'], user_arrow_button_matrix_ch_8=self._midimap[b'User_Arrow_Button_Matrix_Ch_8'], user_arrow_button_matrix_ch_14=self._midimap[b'User_Arrow_Button_Matrix_Ch_14'], user_arrow_button_matrix_ch_15=self._midimap[b'User_Arrow_Button_Matrix_Ch_15'], user_arrow_button_matrix_ch_16=self._midimap[b'User_Arrow_Button_Matrix_Ch_16']))
        self._user_matrix_component.set_enabled(True)

    @subject_slot(b'drum_group')
    def _on_drum_group_changed(self):
        if self._note_modes.selected_mode == b'drum_mode':
            self._drum_group.set_drum_group_device(self._drum_group_finder.drum_group)
        if self._modes.selected_mode == b'note_mode':
            self._select_note_mode()
        else:
            self.release_controlled_track()
        self._update_note_mode_button(self._drum_group_finder.drum_group is not None)
        return

    def _select_note_mode(self):
        """
        Selects which note mode to use depending on the kind of
        current target track and its device chain.  Will also
        select the target if specified.
        """
        track = self._target_track_component.target_track
        drum_device = self._drum_group_finder.drum_group
        if track is None or track.is_foldable or track in self.song().return_tracks or track == self.song().master_track or track.is_frozen or track.has_audio_input:
            self._note_modes.selected_mode = b'audio_mode'
        else:
            if drum_device:
                self._note_modes.selected_mode = b'drum_mode'
            else:
                self._note_modes.selected_mode = b'chromatic_mode'
        self._modes.update()
        if self._note_modes.selected_mode == b'audio_mode':
            self.release_controlled_track()
        else:
            self.set_controlled_track(self._target_track_component.target_track)
        return

    def _select_target_track(self):
        track = self._target_track_component.target_track
        if track != self.song().view.selected_track:
            self.song().view.selected_track = track

    def _update_note_mode_button(self, focused_track_is_drum_track):
        button = self._midimap[b'Note_Mode_Button']
        if focused_track_is_drum_track:
            button.default_states = {True: b'Mode.Drum.On', False: b'Mode.Drum.Off'}
        else:
            button.default_states = {True: b'Mode.Chromatic.On', False: b'Mode.Chromatic.Off'}
        button.reset_state()
        self._modes.update()

    def _show_playing_clip(self):
        track = None
        if self._use_sel_track():
            track = self.song().view.selected_track
        else:
            track = self._target_track_component.target_track
        if track in self.song().tracks:
            slot_index = track.fired_slot_index
            if slot_index < 0:
                slot_index = track.playing_slot_index
            if slot_index >= 0:
                clip_slot = track.clip_slots[slot_index]
                self.song().view.highlighted_clip_slot = clip_slot
        return

    def _set_clip_actions_type(self):
        self._clip_actions_component.use_selected_track(self._use_sel_track())
        self._clip_actions_component.update()

    def _use_sel_track(self):
        return self._modes.selected_mode == b'device_mode'

    def _should_arm_track(self):
        return self._modes.selected_mode == b'record_arm_mode'

    @subject_slot(b'selected_mode')
    def _on_layout_changed(self, mode):
        if mode == b'note_mode':
            self.set_controlled_track(self._target_track_component.target_track)
        else:
            self.release_controlled_track()
        self._session_record.set_enabled(mode != b'user_mode')

    @subject_slot(b'session_record')
    def _on_session_record_changed(self):
        status = self.song().session_record
        feedback_color = int(self._skin[b'Instrument.FeedbackRecord'] if status else self._skin[b'Instrument.Feedback'])
        self._c_instance.set_feedback_velocity(feedback_color)

    def _clear_send_cache(self):
        with self.component_guard():
            for control in self.controls:
                control.clear_send_cache()

    def _update_hardware(self):
        self._clear_send_cache()
        self.update()

    def _update_global_components(self):
        self._actions_component.update()
        self._session_record.update()
        self._modifier_background_component.update()

    def _layout_setup(self, mode):
        self._layout_switch(mode)
        self._clear_send_cache()
        self._update_global_components()

    def _layout_switch(self, mode):
        prefix = consts.SYSEX_STANDARD_PREFIX + consts.SYSEX_PARAM_BYTE_LAYOUT
        suffix = consts.SYSEX_STANDARD_SUFFIX
        self._send_midi(prefix + mode + suffix)
        self._last_sent_mode_byte = mode

    def port_settings_changed(self):
        self.set_highlighting_session_component(None)
        super(Launchpad_Pro, self).port_settings_changed()
        return

    def on_identified(self):
        self._on_handshake_successful()

    def _send_challenge(self):
        challenge_bytes = []
        for index in range(4):
            challenge_bytes.append(self._challenge >> 8 * index & 127)

        challenge = consts.CHALLENGE_PREFIX + tuple(challenge_bytes) + (247, )
        self._send_midi(challenge)

    def _on_handshake_successful(self):
        self._do_send_midi(consts.LIVE_MODE_SWITCH_REQUEST)
        with self.component_guard():
            self._modes.set_enabled(True)
            self._actions_component.set_enabled(True)
            self._session_record.set_enabled(True)
            self._modifier_background_component.set_enabled(True)
            self._shifted_background.set_enabled(True)
            self.release_controlled_track()
            self.set_feedback_channels(consts.FEEDBACK_CHANNELS)
        if self._last_sent_mode_byte is not None:
            self._layout_setup(self._last_sent_mode_byte)
        self.set_highlighting_session_component(self._session)
        self.update()
        return

    def _is_challenge_response(self, midi_bytes):
        return len(midi_bytes) == 10 and midi_bytes[:7] == consts.SYSEX_STANDARD_PREFIX + consts.SYSEX_CHALLENGE_RESPONSE_BYTE

    def _is_response_valid(self, midi_bytes):
        response = long(midi_bytes[7])
        response += long(midi_bytes[8] << 8)
        return response == Live.Application.encrypt_challenge2(self._challenge)

    def handle_sysex(self, midi_bytes):
        if len(midi_bytes) < 7:
            pass
        else:
            if self._is_challenge_response(midi_bytes) and self._is_response_valid(midi_bytes):
                self._on_handshake_successful()
            else:
                if midi_bytes[6] == consts.SYSEX_STATUS_BYTE_LAYOUT and midi_bytes[7] == consts.NOTE_LAYOUT_SYSEX_BYTE[0]:
                    self._update_hardware()
                else:
                    if midi_bytes[6] in (consts.SYSEX_STATUS_BYTE_MODE,
                     consts.SYSEX_STATUS_BYTE_LAYOUT):
                        pass
                    else:
                        super(Launchpad_Pro, self).handle_sysex(midi_bytes)
# okay decompiling Launchpad_Pro.pyc
