function _slicedToArray(r, e) { return _arrayWithHoles(r) || _iterableToArrayLimit(r, e) || _unsupportedIterableToArray(r, e) || _nonIterableRest(); }
function _nonIterableRest() { throw new TypeError("Invalid attempt to destructure non-iterable instance.\nIn order to be iterable, non-array objects must have a [Symbol.iterator]() method."); }
function _iterableToArrayLimit(r, l) { var t = null == r ? null : "undefined" != typeof Symbol && r[Symbol.iterator] || r["@@iterator"]; if (null != t) { var e, n, i, u, a = [], f = !0, o = !1; try { if (i = (t = t.call(r)).next, 0 === l) { if (Object(t) !== t) return; f = !1; } else for (; !(f = (e = i.call(t)).done) && (a.push(e.value), a.length !== l); f = !0); } catch (r) { o = !0, n = r; } finally { try { if (!f && null != t["return"] && (u = t["return"](), Object(u) !== u)) return; } finally { if (o) throw n; } } return a; } }
function _arrayWithHoles(r) { if (Array.isArray(r)) return r; }
function _createForOfIteratorHelper(r, e) { var t = "undefined" != typeof Symbol && r[Symbol.iterator] || r["@@iterator"]; if (!t) { if (Array.isArray(r) || (t = _unsupportedIterableToArray(r)) || e && r && "number" == typeof r.length) { t && (r = t); var _n = 0, F = function F() {}; return { s: F, n: function n() { return _n >= r.length ? { done: !0 } : { done: !1, value: r[_n++] }; }, e: function e(r) { throw r; }, f: F }; } throw new TypeError("Invalid attempt to iterate non-iterable instance.\nIn order to be iterable, non-array objects must have a [Symbol.iterator]() method."); } var o, a = !0, u = !1; return { s: function s() { t = t.call(r); }, n: function n() { var r = t.next(); return a = r.done, r; }, e: function e(r) { u = !0, o = r; }, f: function f() { try { a || null == t["return"] || t["return"](); } finally { if (u) throw o; } } }; }
function _toConsumableArray(r) { return _arrayWithoutHoles(r) || _iterableToArray(r) || _unsupportedIterableToArray(r) || _nonIterableSpread(); }
function _nonIterableSpread() { throw new TypeError("Invalid attempt to spread non-iterable instance.\nIn order to be iterable, non-array objects must have a [Symbol.iterator]() method."); }
function _unsupportedIterableToArray(r, a) { if (r) { if ("string" == typeof r) return _arrayLikeToArray(r, a); var t = {}.toString.call(r).slice(8, -1); return "Object" === t && r.constructor && (t = r.constructor.name), "Map" === t || "Set" === t ? Array.from(r) : "Arguments" === t || /^(?:Ui|I)nt(?:8|16|32)(?:Clamped)?Array$/.test(t) ? _arrayLikeToArray(r, a) : void 0; } }
function _iterableToArray(r) { if ("undefined" != typeof Symbol && null != r[Symbol.iterator] || null != r["@@iterator"]) return Array.from(r); }
function _arrayWithoutHoles(r) { if (Array.isArray(r)) return _arrayLikeToArray(r); }
function _arrayLikeToArray(r, a) { (null == a || a > r.length) && (a = r.length); for (var e = 0, n = Array(a); e < a; e++) n[e] = r[e]; return n; }
function _defineProperties(e, r) { for (var t = 0; t < r.length; t++) { var o = r[t]; o.enumerable = o.enumerable || !1, o.configurable = !0, "value" in o && (o.writable = !0), Object.defineProperty(e, _toPropertyKey(o.key), o); } }
function _createClass(e, r, t) { return r && _defineProperties(e.prototype, r), t && _defineProperties(e, t), Object.defineProperty(e, "prototype", { writable: !1 }), e; }
function _classCallCheck(a, n) { if (!(a instanceof n)) throw new TypeError("Cannot call a class as a function"); }
function _regeneratorRuntime() { "use strict"; /*! regenerator-runtime -- Copyright (c) 2014-present, Facebook, Inc. -- license (MIT): https://github.com/facebook/regenerator/blob/main/LICENSE */ _regeneratorRuntime = function _regeneratorRuntime() { return e; }; var t, e = {}, r = Object.prototype, n = r.hasOwnProperty, o = Object.defineProperty || function (t, e, r) { t[e] = r.value; }, i = "function" == typeof Symbol ? Symbol : {}, a = i.iterator || "@@iterator", c = i.asyncIterator || "@@asyncIterator", u = i.toStringTag || "@@toStringTag"; function define(t, e, r) { return Object.defineProperty(t, e, { value: r, enumerable: !0, configurable: !0, writable: !0 }), t[e]; } try { define({}, ""); } catch (t) { define = function define(t, e, r) { return t[e] = r; }; } function wrap(t, e, r, n) { var i = e && e.prototype instanceof Generator ? e : Generator, a = Object.create(i.prototype), c = new Context(n || []); return o(a, "_invoke", { value: makeInvokeMethod(t, r, c) }), a; } function tryCatch(t, e, r) { try { return { type: "normal", arg: t.call(e, r) }; } catch (t) { return { type: "throw", arg: t }; } } e.wrap = wrap; var h = "suspendedStart", l = "suspendedYield", f = "executing", s = "completed", y = {}; function Generator() {} function GeneratorFunction() {} function GeneratorFunctionPrototype() {} var p = {}; define(p, a, function () { return this; }); var d = Object.getPrototypeOf, v = d && d(d(values([]))); v && v !== r && n.call(v, a) && (p = v); var g = GeneratorFunctionPrototype.prototype = Generator.prototype = Object.create(p); function defineIteratorMethods(t) { ["next", "throw", "return"].forEach(function (e) { define(t, e, function (t) { return this._invoke(e, t); }); }); } function AsyncIterator(t, e) { function invoke(r, o, i, a) { var c = tryCatch(t[r], t, o); if ("throw" !== c.type) { var u = c.arg, h = u.value; return h && "object" == _typeof(h) && n.call(h, "__await") ? e.resolve(h.__await).then(function (t) { invoke("next", t, i, a); }, function (t) { invoke("throw", t, i, a); }) : e.resolve(h).then(function (t) { u.value = t, i(u); }, function (t) { return invoke("throw", t, i, a); }); } a(c.arg); } var r; o(this, "_invoke", { value: function value(t, n) { function callInvokeWithMethodAndArg() { return new e(function (e, r) { invoke(t, n, e, r); }); } return r = r ? r.then(callInvokeWithMethodAndArg, callInvokeWithMethodAndArg) : callInvokeWithMethodAndArg(); } }); } function makeInvokeMethod(e, r, n) { var o = h; return function (i, a) { if (o === f) throw Error("Generator is already running"); if (o === s) { if ("throw" === i) throw a; return { value: t, done: !0 }; } for (n.method = i, n.arg = a;;) { var c = n.delegate; if (c) { var u = maybeInvokeDelegate(c, n); if (u) { if (u === y) continue; return u; } } if ("next" === n.method) n.sent = n._sent = n.arg;else if ("throw" === n.method) { if (o === h) throw o = s, n.arg; n.dispatchException(n.arg); } else "return" === n.method && n.abrupt("return", n.arg); o = f; var p = tryCatch(e, r, n); if ("normal" === p.type) { if (o = n.done ? s : l, p.arg === y) continue; return { value: p.arg, done: n.done }; } "throw" === p.type && (o = s, n.method = "throw", n.arg = p.arg); } }; } function maybeInvokeDelegate(e, r) { var n = r.method, o = e.iterator[n]; if (o === t) return r.delegate = null, "throw" === n && e.iterator["return"] && (r.method = "return", r.arg = t, maybeInvokeDelegate(e, r), "throw" === r.method) || "return" !== n && (r.method = "throw", r.arg = new TypeError("The iterator does not provide a '" + n + "' method")), y; var i = tryCatch(o, e.iterator, r.arg); if ("throw" === i.type) return r.method = "throw", r.arg = i.arg, r.delegate = null, y; var a = i.arg; return a ? a.done ? (r[e.resultName] = a.value, r.next = e.nextLoc, "return" !== r.method && (r.method = "next", r.arg = t), r.delegate = null, y) : a : (r.method = "throw", r.arg = new TypeError("iterator result is not an object"), r.delegate = null, y); } function pushTryEntry(t) { var e = { tryLoc: t[0] }; 1 in t && (e.catchLoc = t[1]), 2 in t && (e.finallyLoc = t[2], e.afterLoc = t[3]), this.tryEntries.push(e); } function resetTryEntry(t) { var e = t.completion || {}; e.type = "normal", delete e.arg, t.completion = e; } function Context(t) { this.tryEntries = [{ tryLoc: "root" }], t.forEach(pushTryEntry, this), this.reset(!0); } function values(e) { if (e || "" === e) { var r = e[a]; if (r) return r.call(e); if ("function" == typeof e.next) return e; if (!isNaN(e.length)) { var o = -1, i = function next() { for (; ++o < e.length;) if (n.call(e, o)) return next.value = e[o], next.done = !1, next; return next.value = t, next.done = !0, next; }; return i.next = i; } } throw new TypeError(_typeof(e) + " is not iterable"); } return GeneratorFunction.prototype = GeneratorFunctionPrototype, o(g, "constructor", { value: GeneratorFunctionPrototype, configurable: !0 }), o(GeneratorFunctionPrototype, "constructor", { value: GeneratorFunction, configurable: !0 }), GeneratorFunction.displayName = define(GeneratorFunctionPrototype, u, "GeneratorFunction"), e.isGeneratorFunction = function (t) { var e = "function" == typeof t && t.constructor; return !!e && (e === GeneratorFunction || "GeneratorFunction" === (e.displayName || e.name)); }, e.mark = function (t) { return Object.setPrototypeOf ? Object.setPrototypeOf(t, GeneratorFunctionPrototype) : (t.__proto__ = GeneratorFunctionPrototype, define(t, u, "GeneratorFunction")), t.prototype = Object.create(g), t; }, e.awrap = function (t) { return { __await: t }; }, defineIteratorMethods(AsyncIterator.prototype), define(AsyncIterator.prototype, c, function () { return this; }), e.AsyncIterator = AsyncIterator, e.async = function (t, r, n, o, i) { void 0 === i && (i = Promise); var a = new AsyncIterator(wrap(t, r, n, o), i); return e.isGeneratorFunction(r) ? a : a.next().then(function (t) { return t.done ? t.value : a.next(); }); }, defineIteratorMethods(g), define(g, u, "Generator"), define(g, a, function () { return this; }), define(g, "toString", function () { return "[object Generator]"; }), e.keys = function (t) { var e = Object(t), r = []; for (var n in e) r.push(n); return r.reverse(), function next() { for (; r.length;) { var t = r.pop(); if (t in e) return next.value = t, next.done = !1, next; } return next.done = !0, next; }; }, e.values = values, Context.prototype = { constructor: Context, reset: function reset(e) { if (this.prev = 0, this.next = 0, this.sent = this._sent = t, this.done = !1, this.delegate = null, this.method = "next", this.arg = t, this.tryEntries.forEach(resetTryEntry), !e) for (var r in this) "t" === r.charAt(0) && n.call(this, r) && !isNaN(+r.slice(1)) && (this[r] = t); }, stop: function stop() { this.done = !0; var t = this.tryEntries[0].completion; if ("throw" === t.type) throw t.arg; return this.rval; }, dispatchException: function dispatchException(e) { if (this.done) throw e; var r = this; function handle(n, o) { return a.type = "throw", a.arg = e, r.next = n, o && (r.method = "next", r.arg = t), !!o; } for (var o = this.tryEntries.length - 1; o >= 0; --o) { var i = this.tryEntries[o], a = i.completion; if ("root" === i.tryLoc) return handle("end"); if (i.tryLoc <= this.prev) { var c = n.call(i, "catchLoc"), u = n.call(i, "finallyLoc"); if (c && u) { if (this.prev < i.catchLoc) return handle(i.catchLoc, !0); if (this.prev < i.finallyLoc) return handle(i.finallyLoc); } else if (c) { if (this.prev < i.catchLoc) return handle(i.catchLoc, !0); } else { if (!u) throw Error("try statement without catch or finally"); if (this.prev < i.finallyLoc) return handle(i.finallyLoc); } } } }, abrupt: function abrupt(t, e) { for (var r = this.tryEntries.length - 1; r >= 0; --r) { var o = this.tryEntries[r]; if (o.tryLoc <= this.prev && n.call(o, "finallyLoc") && this.prev < o.finallyLoc) { var i = o; break; } } i && ("break" === t || "continue" === t) && i.tryLoc <= e && e <= i.finallyLoc && (i = null); var a = i ? i.completion : {}; return a.type = t, a.arg = e, i ? (this.method = "next", this.next = i.finallyLoc, y) : this.complete(a); }, complete: function complete(t, e) { if ("throw" === t.type) throw t.arg; return "break" === t.type || "continue" === t.type ? this.next = t.arg : "return" === t.type ? (this.rval = this.arg = t.arg, this.method = "return", this.next = "end") : "normal" === t.type && e && (this.next = e), y; }, finish: function finish(t) { for (var e = this.tryEntries.length - 1; e >= 0; --e) { var r = this.tryEntries[e]; if (r.finallyLoc === t) return this.complete(r.completion, r.afterLoc), resetTryEntry(r), y; } }, "catch": function _catch(t) { for (var e = this.tryEntries.length - 1; e >= 0; --e) { var r = this.tryEntries[e]; if (r.tryLoc === t) { var n = r.completion; if ("throw" === n.type) { var o = n.arg; resetTryEntry(r); } return o; } } throw Error("illegal catch attempt"); }, delegateYield: function delegateYield(e, r, n) { return this.delegate = { iterator: values(e), resultName: r, nextLoc: n }, "next" === this.method && (this.arg = t), y; } }, e; }
function asyncGeneratorStep(n, t, e, r, o, a, c) { try { var i = n[a](c), u = i.value; } catch (n) { return void e(n); } i.done ? t(u) : Promise.resolve(u).then(r, o); }
function _asyncToGenerator(n) { return function () { var t = this, e = arguments; return new Promise(function (r, o) { var a = n.apply(t, e); function _next(n) { asyncGeneratorStep(a, r, o, _next, _throw, "next", n); } function _throw(n) { asyncGeneratorStep(a, r, o, _next, _throw, "throw", n); } _next(void 0); }); }; }
function ownKeys(e, r) { var t = Object.keys(e); if (Object.getOwnPropertySymbols) { var o = Object.getOwnPropertySymbols(e); r && (o = o.filter(function (r) { return Object.getOwnPropertyDescriptor(e, r).enumerable; })), t.push.apply(t, o); } return t; }
function _objectSpread(e) { for (var r = 1; r < arguments.length; r++) { var t = null != arguments[r] ? arguments[r] : {}; r % 2 ? ownKeys(Object(t), !0).forEach(function (r) { _defineProperty(e, r, t[r]); }) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(t)) : ownKeys(Object(t)).forEach(function (r) { Object.defineProperty(e, r, Object.getOwnPropertyDescriptor(t, r)); }); } return e; }
function _defineProperty(e, r, t) { return (r = _toPropertyKey(r)) in e ? Object.defineProperty(e, r, { value: t, enumerable: !0, configurable: !0, writable: !0 }) : e[r] = t, e; }
function _toPropertyKey(t) { var i = _toPrimitive(t, "string"); return "symbol" == _typeof(i) ? i : i + ""; }
function _toPrimitive(t, r) { if ("object" != _typeof(t) || !t) return t; var e = t[Symbol.toPrimitive]; if (void 0 !== e) { var i = e.call(t, r || "default"); if ("object" != _typeof(i)) return i; throw new TypeError("@@toPrimitive must return a primitive value."); } return ("string" === r ? String : Number)(t); }
function _typeof(o) { "@babel/helpers - typeof"; return _typeof = "function" == typeof Symbol && "symbol" == typeof Symbol.iterator ? function (o) { return typeof o; } : function (o) { return o && "function" == typeof Symbol && o.constructor === Symbol && o !== Symbol.prototype ? "symbol" : typeof o; }, _typeof(o); }
// include: shell.js
// The Module object: Our interface to the outside world. We import
// and export values on it. There are various ways Module can be used:
// 1. Not defined. We create it here
// 2. A function parameter, function(moduleArg) => Promise<Module>
// 3. pre-run appended it, var Module = {}; ..generated code..
// 4. External script tag defines var Module.
// We need to check if Module already exists (e.g. case 3 above).
// Substitution will be replaced with actual code on later stage of the build,
// this way Closure Compiler will not mangle it (e.g. case 4. above).
// Note that if you want to run closure, and also to use Module
// after the generated code, you will need to define   var Module = {};
// before the code. Then that object will be used in the code, and you
// can continue to use Module afterwards as well.
var Module = typeof Module != 'undefined' ? Module : {};

// Determine the runtime environment we are in. You can customize this by
// setting the ENVIRONMENT setting at compile time (see settings.js).

// Attempt to auto-detect the environment
var ENVIRONMENT_IS_WEB = (typeof window === "undefined" ? "undefined" : _typeof(window)) == 'object';
var ENVIRONMENT_IS_WORKER = typeof WorkerGlobalScope != 'undefined';
// N.b. Electron.js environment is simultaneously a NODE-environment, but
// also a web environment.
var ENVIRONMENT_IS_NODE = (typeof process === "undefined" ? "undefined" : _typeof(process)) == 'object' && _typeof(process.versions) == 'object' && typeof process.versions.node == 'string' && process.type != 'renderer';
var ENVIRONMENT_IS_SHELL = !ENVIRONMENT_IS_WEB && !ENVIRONMENT_IS_NODE && !ENVIRONMENT_IS_WORKER;
if (ENVIRONMENT_IS_NODE) {}

// --pre-jses are emitted after the Module integration code, so that they can
// refer to Module (if they choose; they can also define Module)

// Sometimes an existing Module object exists with properties
// meant to overwrite the default module functionality. Here
// we collect those properties and reapply _after_ we configure
// the current environment's defaults to avoid having to be so
// defensive during initialization.
var moduleOverrides = _objectSpread({}, Module);
var arguments_ = [];
var thisProgram = './this.program';
var quit_ = function quit_(status, toThrow) {
  throw toThrow;
};

// `/` should be present at the end if `scriptDirectory` is not empty
var scriptDirectory = '';
function locateFile(path) {
  if (Module['locateFile']) {
    return Module['locateFile'](path, scriptDirectory);
  }
  return scriptDirectory + path;
}

// Hooks that are implemented differently in different runtime environments.
var readAsync, readBinary;
if (ENVIRONMENT_IS_NODE) {
  // These modules will usually be used on Node.js. Load them eagerly to avoid
  // the complexity of lazy-loading.
  var fs = require('fs');
  var nodePath = require('path');
  scriptDirectory = __dirname + '/';

  // include: node_shell_read.js
  readBinary = function readBinary(filename) {
    // We need to re-wrap `file://` strings to URLs.
    filename = isFileURI(filename) ? new URL(filename) : filename;
    var ret = fs.readFileSync(filename);
    return ret;
  };
  readAsync = /*#__PURE__*/function () {
    var _ref = _asyncToGenerator(/*#__PURE__*/_regeneratorRuntime().mark(function _callee(filename) {
      var binary,
        ret,
        _args = arguments;
      return _regeneratorRuntime().wrap(function _callee$(_context) {
        while (1) switch (_context.prev = _context.next) {
          case 0:
            binary = _args.length > 1 && _args[1] !== undefined ? _args[1] : true;
            // See the comment in the `readBinary` function.
            filename = isFileURI(filename) ? new URL(filename) : filename;
            ret = fs.readFileSync(filename, binary ? undefined : 'utf8');
            return _context.abrupt("return", ret);
          case 4:
          case "end":
            return _context.stop();
        }
      }, _callee);
    }));
    return function readAsync(_x) {
      return _ref.apply(this, arguments);
    };
  }();
  // end include: node_shell_read.js
  if (!Module['thisProgram'] && process.argv.length > 1) {
    thisProgram = process.argv[1].replace(/\\/g, '/');
  }
  arguments_ = process.argv.slice(2);
  if (typeof module != 'undefined') {
    module['exports'] = Module;
  }

  // Without this older versions of node (< v15) will log unhandled rejections
  // but return 0, which is not normally the desired behaviour.  This is
  // not be needed with node v15 and about because it is now the default
  // behaviour:
  // See https://nodejs.org/api/cli.html#cli_unhandled_rejections_mode
  var nodeMajor = process.versions.node.split(".")[0];
  if (nodeMajor < 15) {
    process.on('unhandledRejection', function (reason) {
      throw reason;
    });
  }
  quit_ = function quit_(status, toThrow) {
    process.exitCode = status;
    throw toThrow;
  };
} else
  // Note that this includes Node.js workers when relevant (pthreads is enabled).
  // Node.js workers are detected as a combination of ENVIRONMENT_IS_WORKER and
  // ENVIRONMENT_IS_NODE.
  if (ENVIRONMENT_IS_WEB || ENVIRONMENT_IS_WORKER) {
    if (ENVIRONMENT_IS_WORKER) {
      // Check worker, not web, since window could be polyfilled
      scriptDirectory = self.location.href;
    } else if (typeof document != 'undefined' && document.currentScript) {
      // web
      scriptDirectory = document.currentScript.src;
    }
    // blob urls look like blob:http://site.com/etc/etc and we cannot infer anything from them.
    // otherwise, slice off the final part of the url to find the script directory.
    // if scriptDirectory does not contain a slash, lastIndexOf will return -1,
    // and scriptDirectory will correctly be replaced with an empty string.
    // If scriptDirectory contains a query (starting with ?) or a fragment (starting with #),
    // they are removed because they could contain a slash.
    if (scriptDirectory.startsWith('blob:')) {
      scriptDirectory = '';
    } else {
      scriptDirectory = scriptDirectory.slice(0, scriptDirectory.replace(/[?#].*/, '').lastIndexOf('/') + 1);
    }
    {
      // include: web_or_worker_shell_read.js
      if (ENVIRONMENT_IS_WORKER) {
        readBinary = function readBinary(url) {
          var xhr = new XMLHttpRequest();
          xhr.open('GET', url, false);
          xhr.responseType = 'arraybuffer';
          xhr.send(null);
          return new Uint8Array(/** @type{!ArrayBuffer} */xhr.response);
        };
      }
      readAsync = /*#__PURE__*/function () {
        var _ref2 = _asyncToGenerator(/*#__PURE__*/_regeneratorRuntime().mark(function _callee2(url) {
          var response;
          return _regeneratorRuntime().wrap(function _callee2$(_context2) {
            while (1) switch (_context2.prev = _context2.next) {
              case 0:
                if (!isFileURI(url)) {
                  _context2.next = 2;
                  break;
                }
                return _context2.abrupt("return", new Promise(function (resolve, reject) {
                  var xhr = new XMLHttpRequest();
                  xhr.open('GET', url, true);
                  xhr.responseType = 'arraybuffer';
                  xhr.onload = function () {
                    if (xhr.status == 200 || xhr.status == 0 && xhr.response) {
                      // file URLs can return 0
                      resolve(xhr.response);
                      return;
                    }
                    reject(xhr.status);
                  };
                  xhr.onerror = reject;
                  xhr.send(null);
                }));
              case 2:
                _context2.next = 4;
                return fetch(url, {
                  credentials: 'same-origin'
                });
              case 4:
                response = _context2.sent;
                if (!response.ok) {
                  _context2.next = 7;
                  break;
                }
                return _context2.abrupt("return", response.arrayBuffer());
              case 7:
                throw new Error(response.status + ' : ' + response.url);
              case 8:
              case "end":
                return _context2.stop();
            }
          }, _callee2);
        }));
        return function readAsync(_x2) {
          return _ref2.apply(this, arguments);
        };
      }();
      // end include: web_or_worker_shell_read.js
    }
  } else {}
var out = Module['print'] || console.log.bind(console);
var err = Module['printErr'] || console.error.bind(console);

// Merge back in the overrides
Object.assign(Module, moduleOverrides);
// Free the object hierarchy contained in the overrides, this lets the GC
// reclaim data used.
moduleOverrides = null;

// Emit code to handle expected values on the Module object. This applies Module.x
// to the proper local x. This has two benefits: first, we only emit it if it is
// expected to arrive, and second, by using a local everywhere else that can be
// minified.

if (Module['arguments']) arguments_ = Module['arguments'];
if (Module['thisProgram']) thisProgram = Module['thisProgram'];

// perform assertions in shell.js after we set up out() and err(), as otherwise if an assertion fails it cannot print the message
// end include: shell.js

// include: preamble.js
// === Preamble library stuff ===

// Documentation for the public APIs defined in this file must be updated in:
//    site/source/docs/api_reference/preamble.js.rst
// A prebuilt local version of the documentation is available at:
//    site/build/text/docs/api_reference/preamble.js.txt
// You can also build docs locally as HTML or other formats in site/
// An online HTML version (which may be of a different version of Emscripten)
//    is up at http://kripken.github.io/emscripten-site/docs/api_reference/preamble.js.html

var wasmBinary = Module['wasmBinary'];

// Wasm globals

var wasmMemory;

//========================================
// Runtime essentials
//========================================

// whether we are quitting the application. no code should run after this.
// set in exit() and abort()
var ABORT = false;

// set by exit() and abort().  Passed to 'onExit' handler.
// NOTE: This is also used as the process return code code in shell environments
// but only when noExitRuntime is false.
var EXITSTATUS;

// In STRICT mode, we only define assert() when ASSERTIONS is set.  i.e. we
// don't define it at all in release modes.  This matches the behaviour of
// MINIMAL_RUNTIME.
// TODO(sbc): Make this the default even without STRICT enabled.
/** @type {function(*, string=)} */
function assert(condition, text) {
  if (!condition) {
    // This build was created without ASSERTIONS defined.  `assert()` should not
    // ever be called in this configuration but in case there are callers in
    // the wild leave this simple abort() implementation here for now.
    abort(text);
  }
}

// Memory management

var HEAP, /** @type {!Int8Array} */
  HEAP8, /** @type {!Uint8Array} */
  HEAPU8, /** @type {!Int16Array} */
  HEAP16, /** @type {!Uint16Array} */
  HEAPU16, /** @type {!Int32Array} */
  HEAP32, /** @type {!Uint32Array} */
  HEAPU32, /** @type {!Float32Array} */
  HEAPF32, /** @type {!Float64Array} */
  HEAPF64;
var runtimeInitialized = false;

/**
 * Indicates whether filename is delivered via file protocol (as opposed to http/https)
 * @noinline
 */
var isFileURI = function isFileURI(filename) {
  return filename.startsWith('file://');
};

// include: runtime_shared.js
// include: runtime_stack_check.js
// end include: runtime_stack_check.js
// include: runtime_exceptions.js
// end include: runtime_exceptions.js
// include: runtime_debug.js
// end include: runtime_debug.js
// include: memoryprofiler.js
// end include: memoryprofiler.js

function updateMemoryViews() {
  var b = wasmMemory.buffer;
  Module['HEAP8'] = HEAP8 = new Int8Array(b);
  Module['HEAP16'] = HEAP16 = new Int16Array(b);
  Module['HEAPU8'] = HEAPU8 = new Uint8Array(b);
  Module['HEAPU16'] = HEAPU16 = new Uint16Array(b);
  Module['HEAP32'] = HEAP32 = new Int32Array(b);
  Module['HEAPU32'] = HEAPU32 = new Uint32Array(b);
  Module['HEAPF32'] = HEAPF32 = new Float32Array(b);
  Module['HEAPF64'] = HEAPF64 = new Float64Array(b);
}

// The performance global was added to node in v16.0.0:
// https://nodejs.org/api/globals.html#performance
if (ENVIRONMENT_IS_NODE) {
  var _global, _global$performance;
  // This is needed for emscripten_get_now and for pthreads support which
  // depends on it for accurate timing.
  // Use `global` rather than `globalThis` here since older versions of node
  // don't have `globalThis`.
  (_global$performance = (_global = global).performance) !== null && _global$performance !== void 0 ? _global$performance : _global.performance = require('perf_hooks').performance;
}
// end include: runtime_shared.js
function preRun() {
  if (Module['preRun']) {
    if (typeof Module['preRun'] == 'function') Module['preRun'] = [Module['preRun']];
    while (Module['preRun'].length) {
      addOnPreRun(Module['preRun'].shift());
    }
  }
  callRuntimeCallbacks(onPreRuns);
}
function initRuntime() {
  runtimeInitialized = true;
  if (!Module['noFSInit'] && !FS.initialized) FS.init();
  TTY.init();
  wasmExports['__wasm_call_ctors']();
  FS.ignorePermissions = false;
}
function preMain() {}
function postRun() {
  if (Module['postRun']) {
    if (typeof Module['postRun'] == 'function') Module['postRun'] = [Module['postRun']];
    while (Module['postRun'].length) {
      addOnPostRun(Module['postRun'].shift());
    }
  }
  callRuntimeCallbacks(onPostRuns);
}

// A counter of dependencies for calling run(). If we need to
// do asynchronous work before running, increment this and
// decrement it. Incrementing must happen in a place like
// Module.preRun (used by emcc to add file preloading).
// Note that you can add dependencies in preRun, even though
// it happens right before run - run will be postponed until
// the dependencies are met.
var runDependencies = 0;
var dependenciesFulfilled = null; // overridden to take different actions when all run dependencies are fulfilled

function getUniqueRunDependency(id) {
  return id;
}
function addRunDependency(id) {
  var _Module$monitorRunDep;
  runDependencies++;
  (_Module$monitorRunDep = Module['monitorRunDependencies']) === null || _Module$monitorRunDep === void 0 || _Module$monitorRunDep.call(Module, runDependencies);
}
function removeRunDependency(id) {
  var _Module$monitorRunDep2;
  runDependencies--;
  (_Module$monitorRunDep2 = Module['monitorRunDependencies']) === null || _Module$monitorRunDep2 === void 0 || _Module$monitorRunDep2.call(Module, runDependencies);
  if (runDependencies == 0) {
    if (dependenciesFulfilled) {
      var callback = dependenciesFulfilled;
      dependenciesFulfilled = null;
      callback(); // can add another dependenciesFulfilled
    }
  }
}

/** @param {string|number=} what */
function abort(what) {
  var _Module$onAbort;
  (_Module$onAbort = Module['onAbort']) === null || _Module$onAbort === void 0 || _Module$onAbort.call(Module, what);
  what = 'Aborted(' + what + ')';
  // TODO(sbc): Should we remove printing and leave it up to whoever
  // catches the exception?
  err(what);
  ABORT = true;
  what += '. Build with -sASSERTIONS for more info.';

  // Use a wasm runtime error, because a JS error might be seen as a foreign
  // exception, which means we'd run destructors on it. We need the error to
  // simply make the program stop.
  // FIXME This approach does not work in Wasm EH because it currently does not assume
  // all RuntimeErrors are from traps; it decides whether a RuntimeError is from
  // a trap or not based on a hidden field within the object. So at the moment
  // we don't have a way of throwing a wasm trap from JS. TODO Make a JS API that
  // allows this in the wasm spec.

  // Suppress closure compiler warning here. Closure compiler's builtin extern
  // definition for WebAssembly.RuntimeError claims it takes no arguments even
  // though it can.
  // TODO(https://github.com/google/closure-compiler/pull/3913): Remove if/when upstream closure gets fixed.
  /** @suppress {checkTypes} */
  var e = new WebAssembly.RuntimeError(what);

  // Throw the error whether or not MODULARIZE is set because abort is used
  // in code paths apart from instantiation where an exception is expected
  // to be thrown when abort is called.
  throw e;
}
var wasmBinaryFile;
function findWasmBinary() {
  return locateFile('baye.v2.wasm');
}
function getBinarySync(file) {
  if (file == wasmBinaryFile && wasmBinary) {
    return new Uint8Array(wasmBinary);
  }
  if (readBinary) {
    return readBinary(file);
  }
  throw 'both async and sync fetching of the wasm failed';
}
function getWasmBinary(_x3) {
  return _getWasmBinary.apply(this, arguments);
}
function _getWasmBinary() {
  _getWasmBinary = _asyncToGenerator(/*#__PURE__*/_regeneratorRuntime().mark(function _callee4(binaryFile) {
    var response;
    return _regeneratorRuntime().wrap(function _callee4$(_context4) {
      while (1) switch (_context4.prev = _context4.next) {
        case 0:
          if (wasmBinary) {
            _context4.next = 10;
            break;
          }
          _context4.prev = 1;
          _context4.next = 4;
          return readAsync(binaryFile);
        case 4:
          response = _context4.sent;
          return _context4.abrupt("return", new Uint8Array(response));
        case 8:
          _context4.prev = 8;
          _context4.t0 = _context4["catch"](1);
        case 10:
          return _context4.abrupt("return", getBinarySync(binaryFile));
        case 11:
        case "end":
          return _context4.stop();
      }
    }, _callee4, null, [[1, 8]]);
  }));
  return _getWasmBinary.apply(this, arguments);
}
function instantiateArrayBuffer(_x4, _x5) {
  return _instantiateArrayBuffer.apply(this, arguments);
}
function _instantiateArrayBuffer() {
  _instantiateArrayBuffer = _asyncToGenerator(/*#__PURE__*/_regeneratorRuntime().mark(function _callee5(binaryFile, imports) {
    var binary, instance;
    return _regeneratorRuntime().wrap(function _callee5$(_context5) {
      while (1) switch (_context5.prev = _context5.next) {
        case 0:
          _context5.prev = 0;
          _context5.next = 3;
          return getWasmBinary(binaryFile);
        case 3:
          binary = _context5.sent;
          _context5.next = 6;
          return WebAssembly.instantiate(binary, imports);
        case 6:
          instance = _context5.sent;
          return _context5.abrupt("return", instance);
        case 10:
          _context5.prev = 10;
          _context5.t0 = _context5["catch"](0);
          err("failed to asynchronously prepare wasm: ".concat(_context5.t0));
          abort(_context5.t0);
        case 14:
        case "end":
          return _context5.stop();
      }
    }, _callee5, null, [[0, 10]]);
  }));
  return _instantiateArrayBuffer.apply(this, arguments);
}
function instantiateAsync(_x6, _x7, _x8) {
  return _instantiateAsync.apply(this, arguments);
}
function _instantiateAsync() {
  _instantiateAsync = _asyncToGenerator(/*#__PURE__*/_regeneratorRuntime().mark(function _callee6(binary, binaryFile, imports) {
    var response, instantiationResult;
    return _regeneratorRuntime().wrap(function _callee6$(_context6) {
      while (1) switch (_context6.prev = _context6.next) {
        case 0:
          if (!(!binary && typeof WebAssembly.instantiateStreaming == 'function'
          // Don't use streaming for file:// delivered objects in a webview, fetch them synchronously.
          && !isFileURI(binaryFile)
          // Avoid instantiateStreaming() on Node.js environment for now, as while
          // Node.js v18.1.0 implements it, it does not have a full fetch()
          // implementation yet.
          //
          // Reference:
          //   https://github.com/emscripten-core/emscripten/pull/16917
          && !ENVIRONMENT_IS_NODE)) {
            _context6.next = 14;
            break;
          }
          _context6.prev = 1;
          response = fetch(binaryFile, {
            credentials: 'same-origin'
          });
          _context6.next = 5;
          return WebAssembly.instantiateStreaming(response, imports);
        case 5:
          instantiationResult = _context6.sent;
          return _context6.abrupt("return", instantiationResult);
        case 9:
          _context6.prev = 9;
          _context6.t0 = _context6["catch"](1);
          // We expect the most common failure cause to be a bad MIME type for the binary,
          // in which case falling back to ArrayBuffer instantiation should work.
          err("wasm streaming compile failed: ".concat(_context6.t0));
          err('falling back to ArrayBuffer instantiation');
          // fall back of instantiateArrayBuffer below
        case 13:
          ;
        case 14:
          return _context6.abrupt("return", instantiateArrayBuffer(binaryFile, imports));
        case 15:
        case "end":
          return _context6.stop();
      }
    }, _callee6, null, [[1, 9]]);
  }));
  return _instantiateAsync.apply(this, arguments);
}
function getWasmImports() {
  // prepare imports
  return {
    'env': wasmImports,
    'wasi_snapshot_preview1': wasmImports
  };
}

// Create the wasm instance.
// Receives the wasm imports, returns the exports.
function createWasm() {
  return _createWasm.apply(this, arguments);
} // Globals used by JS i64 conversions (see makeSetValue)
function _createWasm() {
  _createWasm = _asyncToGenerator(/*#__PURE__*/_regeneratorRuntime().mark(function _callee7() {
    var receiveInstance, receiveInstantiationResult, info, result, exports;
    return _regeneratorRuntime().wrap(function _callee7$(_context7) {
      while (1) switch (_context7.prev = _context7.next) {
        case 0:
          receiveInstantiationResult = function _receiveInstantiation(result) {
            // 'result' is a ResultObject object which has both the module and instance.
            // receiveInstance() will swap in the exports (to Module.asm) so they can be called
            // TODO: Due to Closure regression https://github.com/google/closure-compiler/issues/3193, the above line no longer optimizes out down to the following line.
            // When the regression is fixed, can restore the above PTHREADS-enabled path.
            return receiveInstance(result['instance']);
          };
          receiveInstance = function _receiveInstance(instance, module) {
            wasmExports = instance.exports;
            wasmExports = Asyncify.instrumentWasmExports(wasmExports);
            wasmMemory = wasmExports['memory'];
            updateMemoryViews();
            removeRunDependency('wasm-instantiate');
            return wasmExports;
          }; // Load the wasm module and create an instance of using native support in the JS engine.
          // handle a generated wasm instance, receiving its exports and
          // performing other necessary setup
          /** @param {WebAssembly.Module=} module*/
          // wait for the pthread pool (if any)
          addRunDependency('wasm-instantiate');

          // Prefer streaming instantiation if available.
          info = getWasmImports(); // User shell pages can write their own Module.instantiateWasm = function(imports, successCallback) callback
          // to manually instantiate the Wasm module themselves. This allows pages to
          // run the instantiation parallel to any other async startup actions they are
          // performing.
          // Also pthreads and wasm workers initialize the wasm instance through this
          // path.
          if (!Module['instantiateWasm']) {
            _context7.next = 6;
            break;
          }
          return _context7.abrupt("return", new Promise(function (resolve, reject) {
            Module['instantiateWasm'](info, function (mod, inst) {
              receiveInstance(mod, inst);
              resolve(mod.exports);
            });
          }));
        case 6:
          wasmBinaryFile !== null && wasmBinaryFile !== void 0 ? wasmBinaryFile : wasmBinaryFile = findWasmBinary();
          _context7.next = 9;
          return instantiateAsync(wasmBinary, wasmBinaryFile, info);
        case 9:
          result = _context7.sent;
          exports = receiveInstantiationResult(result);
          return _context7.abrupt("return", exports);
        case 12:
        case "end":
          return _context7.stop();
      }
    }, _callee7);
  }));
  return _createWasm.apply(this, arguments);
}
var tempDouble;
var tempI64;

// end include: preamble.js

// Begin JS library code
var ExitStatus = /*#__PURE__*/_createClass(function ExitStatus(status) {
  "use strict";

  _classCallCheck(this, ExitStatus);
  _defineProperty(this, "name", 'ExitStatus');
  this.message = "Program terminated with exit(".concat(status, ")");
  this.status = status;
});
var callRuntimeCallbacks = function callRuntimeCallbacks(callbacks) {
  while (callbacks.length > 0) {
    // Pass the module as the first argument.
    callbacks.shift()(Module);
  }
};
var onPostRuns = [];
var addOnPostRun = function addOnPostRun(cb) {
  return onPostRuns.unshift(cb);
};
var onPreRuns = [];
var addOnPreRun = function addOnPreRun(cb) {
  return onPreRuns.unshift(cb);
};

/**
 * @param {number} ptr
 * @param {string} type
 */
function getValue(ptr) {
  var type = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : 'i8';
  if (type.endsWith('*')) type = '*';
  switch (type) {
    case 'i1':
      return HEAP8[ptr];
    case 'i8':
      return HEAP8[ptr];
    case 'i16':
      return HEAP16[ptr >> 1];
    case 'i32':
      return HEAP32[ptr >> 2];
    case 'i64':
      abort('to do getValue(i64) use WASM_BIGINT');
    case 'float':
      return HEAPF32[ptr >> 2];
    case 'double':
      return HEAPF64[ptr >> 3];
    case '*':
      return HEAPU32[ptr >> 2];
    default:
      abort("invalid type for getValue: ".concat(type));
  }
}
var noExitRuntime = Module['noExitRuntime'] || true;

/**
 * @param {number} ptr
 * @param {number} value
 * @param {string} type
 */
function setValue(ptr, value) {
  var type = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : 'i8';
  if (type.endsWith('*')) type = '*';
  switch (type) {
    case 'i1':
      HEAP8[ptr] = value;
      break;
    case 'i8':
      HEAP8[ptr] = value;
      break;
    case 'i16':
      HEAP16[ptr >> 1] = value;
      break;
    case 'i32':
      HEAP32[ptr >> 2] = value;
      break;
    case 'i64':
      abort('to do setValue(i64) use WASM_BIGINT');
    case 'float':
      HEAPF32[ptr >> 2] = value;
      break;
    case 'double':
      HEAPF64[ptr >> 3] = value;
      break;
    case '*':
      HEAPU32[ptr >> 2] = value;
      break;
    default:
      abort("invalid type for setValue: ".concat(type));
  }
}
var stackRestore = function stackRestore(val) {
  return _emscripten_stack_restore(val);
};
var stackSave = function stackSave() {
  return _emscripten_stack_get_current2();
};
var __abort_js = function __abort_js() {
  return abort('');
};
var readEmAsmArgsArray = [];
var readEmAsmArgs = function readEmAsmArgs(sigPtr, buf) {
  readEmAsmArgsArray.length = 0;
  var ch;
  // Most arguments are i32s, so shift the buffer pointer so it is a plain
  // index into HEAP32.
  while (ch = HEAPU8[sigPtr++]) {
    // Floats are always passed as doubles, so all types except for 'i'
    // are 8 bytes and require alignment.
    var wide = ch != 105;
    wide &= ch != 112;
    buf += wide && buf % 8 ? 4 : 0;
    readEmAsmArgsArray.push(
    // Special case for pointers under wasm64 or CAN_ADDRESS_2GB mode.
    ch == 112 ? HEAPU32[buf >> 2] : ch == 105 ? HEAP32[buf >> 2] : HEAPF64[buf >> 3]);
    buf += wide ? 8 : 4;
  }
  return readEmAsmArgsArray;
};
var runEmAsmFunction = function runEmAsmFunction(code, sigPtr, argbuf) {
  var args = readEmAsmArgs(sigPtr, argbuf);
  return ASM_CONSTS[code].apply(ASM_CONSTS, _toConsumableArray(args));
};
var _emscripten_asm_const_int = function _emscripten_asm_const_int(code, sigPtr, argbuf) {
  return runEmAsmFunction(code, sigPtr, argbuf);
};
var handleException = function handleException(e) {
  // Certain exception types we do not treat as errors since they are used for
  // internal control flow.
  // 1. ExitStatus, which is thrown by exit()
  // 2. "unwind", which is thrown by emscripten_unwind_to_js_event_loop() and others
  //    that wish to return to JS event loop.
  if (e instanceof ExitStatus || e == 'unwind') {
    return EXITSTATUS;
  }
  quit_(1, e);
};
var runtimeKeepaliveCounter = 0;
var keepRuntimeAlive = function keepRuntimeAlive() {
  return noExitRuntime || runtimeKeepaliveCounter > 0;
};
var _proc_exit = function _proc_exit(code) {
  EXITSTATUS = code;
  if (!keepRuntimeAlive()) {
    var _Module$onExit;
    (_Module$onExit = Module['onExit']) === null || _Module$onExit === void 0 || _Module$onExit.call(Module, code);
    ABORT = true;
  }
  quit_(code, new ExitStatus(code));
};
/** @suppress {duplicate } */
/** @param {boolean|number=} implicit */
var exitJS = function exitJS(status, implicit) {
  EXITSTATUS = status;
  _proc_exit(status);
};
var _exit = exitJS;
var maybeExit = function maybeExit() {
  if (!keepRuntimeAlive()) {
    try {
      _exit(EXITSTATUS);
    } catch (e) {
      handleException(e);
    }
  }
};
var callUserCallback = function callUserCallback(func) {
  if (ABORT) {
    return;
  }
  try {
    func();
    maybeExit();
  } catch (e) {
    handleException(e);
  }
};
/** @param {number=} timeout */
var safeSetTimeout = function safeSetTimeout(func, timeout) {
  return setTimeout(function () {
    callUserCallback(func);
  }, timeout);
};
var _emscripten_set_main_loop_timing = function _emscripten_set_main_loop_timing(mode, value) {
  MainLoop.timingMode = mode;
  MainLoop.timingValue = value;
  if (!MainLoop.func) {
    return 1; // Return non-zero on failure, can't set timing mode when there is no main loop.
  }
  if (!MainLoop.running) {
    MainLoop.running = true;
  }
  if (mode == 0) {
    MainLoop.scheduler = function MainLoop_scheduler_setTimeout() {
      var timeUntilNextTick = Math.max(0, MainLoop.tickStartTime + value - _emscripten_get_now()) | 0;
      setTimeout(MainLoop.runner, timeUntilNextTick); // doing this each time means that on exception, we stop
    };
    MainLoop.method = 'timeout';
  } else if (mode == 1) {
    MainLoop.scheduler = function MainLoop_scheduler_rAF() {
      MainLoop.requestAnimationFrame(MainLoop.runner);
    };
    MainLoop.method = 'rAF';
  } else if (mode == 2) {
    if (typeof MainLoop.setImmediate == 'undefined') {
      if (typeof setImmediate == 'undefined') {
        // Emulate setImmediate. (note: not a complete polyfill, we don't emulate clearImmediate() to keep code size to minimum, since not needed)
        var setImmediates = [];
        var emscriptenMainLoopMessageId = 'setimmediate';
        /** @param {Event} event */
        var MainLoop_setImmediate_messageHandler = function MainLoop_setImmediate_messageHandler(event) {
          // When called in current thread or Worker, the main loop ID is structured slightly different to accommodate for --proxy-to-worker runtime listening to Worker events,
          // so check for both cases.
          if (event.data === emscriptenMainLoopMessageId || event.data.target === emscriptenMainLoopMessageId) {
            event.stopPropagation();
            setImmediates.shift()();
          }
        };
        addEventListener("message", MainLoop_setImmediate_messageHandler, true);
        MainLoop.setImmediate = /** @type{function(function(): ?, ...?): number} */function (func) {
          setImmediates.push(func);
          if (ENVIRONMENT_IS_WORKER) {
            var _setImmediates, _Module$_setImmediate;
            (_Module$_setImmediate = Module[_setImmediates = 'setImmediates']) !== null && _Module$_setImmediate !== void 0 ? _Module$_setImmediate : Module[_setImmediates] = [];
            Module['setImmediates'].push(func);
            postMessage({
              target: emscriptenMainLoopMessageId
            }); // In --proxy-to-worker, route the message via proxyClient.js
          } else postMessage(emscriptenMainLoopMessageId, "*"); // On the main thread, can just send the message to itself.
        };
      } else {
        MainLoop.setImmediate = setImmediate;
      }
    }
    MainLoop.scheduler = function MainLoop_scheduler_setImmediate() {
      MainLoop.setImmediate(MainLoop.runner);
    };
    MainLoop.method = 'immediate';
  }
  return 0;
};
var _emscripten_get_now = function _emscripten_get_now() {
  return performance.now();
};

/**
 * @param {number=} arg
 * @param {boolean=} noSetTiming
 */
var setMainLoop = function setMainLoop(iterFunc, fps, simulateInfiniteLoop, arg, noSetTiming) {
  MainLoop.func = iterFunc;
  MainLoop.arg = arg;
  var thisMainLoopId = MainLoop.currentlyRunningMainloop;
  function checkIsRunning() {
    if (thisMainLoopId < MainLoop.currentlyRunningMainloop) {
      maybeExit();
      return false;
    }
    return true;
  }

  // We create the loop runner here but it is not actually running until
  // _emscripten_set_main_loop_timing is called (which might happen a
  // later time).  This member signifies that the current runner has not
  // yet been started so that we can call runtimeKeepalivePush when it
  // gets it timing set for the first time.
  MainLoop.running = false;
  MainLoop.runner = function MainLoop_runner() {
    if (ABORT) return;
    if (MainLoop.queue.length > 0) {
      var start = Date.now();
      var blocker = MainLoop.queue.shift();
      blocker.func(blocker.arg);
      if (MainLoop.remainingBlockers) {
        var remaining = MainLoop.remainingBlockers;
        var next = remaining % 1 == 0 ? remaining - 1 : Math.floor(remaining);
        if (blocker.counted) {
          MainLoop.remainingBlockers = next;
        } else {
          // not counted, but move the progress along a tiny bit
          next = next + 0.5; // do not steal all the next one's progress
          MainLoop.remainingBlockers = (8 * remaining + next) / 9;
        }
      }
      MainLoop.updateStatus();

      // catches pause/resume main loop from blocker execution
      if (!checkIsRunning()) return;
      setTimeout(MainLoop.runner, 0);
      return;
    }

    // catch pauses from non-main loop sources
    if (!checkIsRunning()) return;

    // Implement very basic swap interval control
    MainLoop.currentFrameNumber = MainLoop.currentFrameNumber + 1 | 0;
    if (MainLoop.timingMode == 1 && MainLoop.timingValue > 1 && MainLoop.currentFrameNumber % MainLoop.timingValue != 0) {
      // Not the scheduled time to render this frame - skip.
      MainLoop.scheduler();
      return;
    } else if (MainLoop.timingMode == 0) {
      MainLoop.tickStartTime = _emscripten_get_now();
    }
    MainLoop.runIter(iterFunc);

    // catch pauses from the main loop itself
    if (!checkIsRunning()) return;
    MainLoop.scheduler();
  };
  if (!noSetTiming) {
    if (fps > 0) {
      _emscripten_set_main_loop_timing(0, 1000.0 / fps);
    } else {
      // Do rAF by rendering each frame (no decimating)
      _emscripten_set_main_loop_timing(1, 1);
    }
    MainLoop.scheduler();
  }
  if (simulateInfiniteLoop) {
    throw 'unwind';
  }
};
var MainLoop = {
  running: false,
  scheduler: null,
  method: "",
  currentlyRunningMainloop: 0,
  func: null,
  arg: 0,
  timingMode: 0,
  timingValue: 0,
  currentFrameNumber: 0,
  queue: [],
  preMainLoop: [],
  postMainLoop: [],
  pause: function pause() {
    MainLoop.scheduler = null;
    // Incrementing this signals the previous main loop that it's now become old, and it must return.
    MainLoop.currentlyRunningMainloop++;
  },
  resume: function resume() {
    MainLoop.currentlyRunningMainloop++;
    var timingMode = MainLoop.timingMode;
    var timingValue = MainLoop.timingValue;
    var func = MainLoop.func;
    MainLoop.func = null;
    // do not set timing and call scheduler, we will do it on the next lines
    setMainLoop(func, 0, false, MainLoop.arg, true);
    _emscripten_set_main_loop_timing(timingMode, timingValue);
    MainLoop.scheduler();
  },
  updateStatus: function updateStatus() {
    if (Module['setStatus']) {
      var _MainLoop$remainingBl, _MainLoop$expectedBlo;
      var message = Module['statusMessage'] || 'Please wait...';
      var remaining = (_MainLoop$remainingBl = MainLoop.remainingBlockers) !== null && _MainLoop$remainingBl !== void 0 ? _MainLoop$remainingBl : 0;
      var expected = (_MainLoop$expectedBlo = MainLoop.expectedBlockers) !== null && _MainLoop$expectedBlo !== void 0 ? _MainLoop$expectedBlo : 0;
      if (remaining) {
        if (remaining < expected) {
          Module['setStatus']("{message} ({expected - remaining}/{expected})");
        } else {
          Module['setStatus'](message);
        }
      } else {
        Module['setStatus']('');
      }
    }
  },
  init: function init() {
    Module['preMainLoop'] && MainLoop.preMainLoop.push(Module['preMainLoop']);
    Module['postMainLoop'] && MainLoop.postMainLoop.push(Module['postMainLoop']);
  },
  runIter: function runIter(func) {
    if (ABORT) return;
    var _iterator = _createForOfIteratorHelper(MainLoop.preMainLoop),
      _step;
    try {
      for (_iterator.s(); !(_step = _iterator.n()).done;) {
        var pre = _step.value;
        if (pre() === false) {
          return; // |return false| skips a frame
        }
      }
    } catch (err) {
      _iterator.e(err);
    } finally {
      _iterator.f();
    }
    callUserCallback(func);
    var _iterator2 = _createForOfIteratorHelper(MainLoop.postMainLoop),
      _step2;
    try {
      for (_iterator2.s(); !(_step2 = _iterator2.n()).done;) {
        var post = _step2.value;
        post();
      }
    } catch (err) {
      _iterator2.e(err);
    } finally {
      _iterator2.f();
    }
  },
  nextRAF: 0,
  fakeRequestAnimationFrame: function fakeRequestAnimationFrame(func) {
    // try to keep 60fps between calls to here
    var now = Date.now();
    if (MainLoop.nextRAF === 0) {
      MainLoop.nextRAF = now + 1000 / 60;
    } else {
      while (now + 2 >= MainLoop.nextRAF) {
        // fudge a little, to avoid timer jitter causing us to do lots of delay:0
        MainLoop.nextRAF += 1000 / 60;
      }
    }
    var delay = Math.max(MainLoop.nextRAF - now, 0);
    setTimeout(func, delay);
  },
  requestAnimationFrame: function (_requestAnimationFrame) {
    function requestAnimationFrame(_x9) {
      return _requestAnimationFrame.apply(this, arguments);
    }
    requestAnimationFrame.toString = function () {
      return _requestAnimationFrame.toString();
    };
    return requestAnimationFrame;
  }(function (func) {
    if (typeof requestAnimationFrame == 'function') {
      requestAnimationFrame(func);
      return;
    }
    var RAF = MainLoop.fakeRequestAnimationFrame;
    if (typeof window != 'undefined') {
      RAF = window['requestAnimationFrame'] || window['mozRequestAnimationFrame'] || window['webkitRequestAnimationFrame'] || window['msRequestAnimationFrame'] || window['oRequestAnimationFrame'] || RAF;
    }
    RAF(func);
  })
};
var safeRequestAnimationFrame = function safeRequestAnimationFrame(func) {
  return MainLoop.requestAnimationFrame(function () {
    callUserCallback(func);
  });
};
var _emscripten_async_call = function _emscripten_async_call(func, arg, millis) {
  var wrapper = function wrapper() {
    return function (a1) {
      return dynCall_vi(func, a1);
    }(arg);
  };
  if (millis >= 0
  // node does not support requestAnimationFrame
  || ENVIRONMENT_IS_NODE) {
    safeSetTimeout(wrapper, millis);
  } else {
    safeRequestAnimationFrame(wrapper);
  }
};
var abortOnCannotGrowMemory = function abortOnCannotGrowMemory(requestedSize) {
  abort('OOM');
};
var _emscripten_resize_heap = function _emscripten_resize_heap(requestedSize) {
  var oldSize = HEAPU8.length;
  // With CAN_ADDRESS_2GB or MEMORY64, pointers are already unsigned.
  requestedSize >>>= 0;
  abortOnCannotGrowMemory(requestedSize);
};
var _emscripten_sleep = function _emscripten_sleep(ms) {
  // emscripten_sleep() does not return a value, but we still need a |return|
  // here for stack switching support (ASYNCIFY=2). In that mode this function
  // returns a Promise instead of nothing, and that Promise is what tells the
  // wasm VM to pause the stack.
  return Asyncify.handleSleep(function (wakeUp) {
    return safeSetTimeout(wakeUp, ms);
  });
};
_emscripten_sleep.isAsync = true;
var PATH = {
  isAbs: function isAbs(path) {
    return path.charAt(0) === '/';
  },
  splitPath: function splitPath(filename) {
    var splitPathRe = /^(\/?|)([\s\S]*?)((?:\.{1,2}|[^\/]+?|)(\.[^.\/]*|))(?:[\/]*)$/;
    return splitPathRe.exec(filename).slice(1);
  },
  normalizeArray: function normalizeArray(parts, allowAboveRoot) {
    // if the path tries to go above the root, `up` ends up > 0
    var up = 0;
    for (var i = parts.length - 1; i >= 0; i--) {
      var last = parts[i];
      if (last === '.') {
        parts.splice(i, 1);
      } else if (last === '..') {
        parts.splice(i, 1);
        up++;
      } else if (up) {
        parts.splice(i, 1);
        up--;
      }
    }
    // if the path is allowed to go above the root, restore leading ..s
    if (allowAboveRoot) {
      for (; up; up--) {
        parts.unshift('..');
      }
    }
    return parts;
  },
  normalize: function normalize(path) {
    var isAbsolute = PATH.isAbs(path),
      trailingSlash = path.slice(-1) === '/';
    // Normalize the path
    path = PATH.normalizeArray(path.split('/').filter(function (p) {
      return !!p;
    }), !isAbsolute).join('/');
    if (!path && !isAbsolute) {
      path = '.';
    }
    if (path && trailingSlash) {
      path += '/';
    }
    return (isAbsolute ? '/' : '') + path;
  },
  dirname: function dirname(path) {
    var result = PATH.splitPath(path),
      root = result[0],
      dir = result[1];
    if (!root && !dir) {
      // No dirname whatsoever
      return '.';
    }
    if (dir) {
      // It has a dirname, strip trailing slash
      dir = dir.slice(0, -1);
    }
    return root + dir;
  },
  basename: function basename(path) {
    return path && path.match(/([^\/]+|\/)\/*$/)[1];
  },
  join: function join() {
    for (var _len = arguments.length, paths = new Array(_len), _key = 0; _key < _len; _key++) {
      paths[_key] = arguments[_key];
    }
    return PATH.normalize(paths.join('/'));
  },
  join2: function join2(l, r) {
    return PATH.normalize(l + '/' + r);
  }
};
var initRandomFill = function initRandomFill() {
  // This block is not needed on v19+ since crypto.getRandomValues is builtin
  if (ENVIRONMENT_IS_NODE) {
    var nodeCrypto = require('crypto');
    return function (view) {
      return nodeCrypto.randomFillSync(view);
    };
  }
  return function (view) {
    return crypto.getRandomValues(view);
  };
};
var _randomFill = function randomFill(view) {
  // Lazily init on the first invocation.
  (_randomFill = initRandomFill())(view);
};
var PATH_FS = {
  resolve: function resolve() {
    var resolvedPath = '',
      resolvedAbsolute = false;
    for (var i = arguments.length - 1; i >= -1 && !resolvedAbsolute; i--) {
      var path = i >= 0 ? i < 0 || arguments.length <= i ? undefined : arguments[i] : FS.cwd();
      // Skip empty and invalid entries
      if (typeof path != 'string') {
        throw new TypeError('Arguments to path.resolve must be strings');
      } else if (!path) {
        return ''; // an invalid portion invalidates the whole thing
      }
      resolvedPath = path + '/' + resolvedPath;
      resolvedAbsolute = PATH.isAbs(path);
    }
    // At this point the path should be resolved to a full absolute path, but
    // handle relative paths to be safe (might happen when process.cwd() fails)
    resolvedPath = PATH.normalizeArray(resolvedPath.split('/').filter(function (p) {
      return !!p;
    }), !resolvedAbsolute).join('/');
    return (resolvedAbsolute ? '/' : '') + resolvedPath || '.';
  },
  relative: function relative(from, to) {
    from = PATH_FS.resolve(from).slice(1);
    to = PATH_FS.resolve(to).slice(1);
    function trim(arr) {
      var start = 0;
      for (; start < arr.length; start++) {
        if (arr[start] !== '') break;
      }
      var end = arr.length - 1;
      for (; end >= 0; end--) {
        if (arr[end] !== '') break;
      }
      if (start > end) return [];
      return arr.slice(start, end - start + 1);
    }
    var fromParts = trim(from.split('/'));
    var toParts = trim(to.split('/'));
    var length = Math.min(fromParts.length, toParts.length);
    var samePartsLength = length;
    for (var i = 0; i < length; i++) {
      if (fromParts[i] !== toParts[i]) {
        samePartsLength = i;
        break;
      }
    }
    var outputParts = [];
    for (var i = samePartsLength; i < fromParts.length; i++) {
      outputParts.push('..');
    }
    outputParts = outputParts.concat(toParts.slice(samePartsLength));
    return outputParts.join('/');
  }
};
var UTF8Decoder = typeof TextDecoder != 'undefined' ? new TextDecoder() : undefined;

/**
 * Given a pointer 'idx' to a null-terminated UTF8-encoded string in the given
 * array that contains uint8 values, returns a copy of that string as a
 * Javascript String object.
 * heapOrArray is either a regular array, or a JavaScript typed array view.
 * @param {number=} idx
 * @param {number=} maxBytesToRead
 * @return {string}
 */
var UTF8ArrayToString = function UTF8ArrayToString(heapOrArray) {
  var idx = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : 0;
  var maxBytesToRead = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : NaN;
  var endIdx = idx + maxBytesToRead;
  var endPtr = idx;
  // TextDecoder needs to know the byte length in advance, it doesn't stop on
  // null terminator by itself.  Also, use the length info to avoid running tiny
  // strings through TextDecoder, since .subarray() allocates garbage.
  // (As a tiny code save trick, compare endPtr against endIdx using a negation,
  // so that undefined/NaN means Infinity)
  while (heapOrArray[endPtr] && !(endPtr >= endIdx)) ++endPtr;
  if (endPtr - idx > 16 && heapOrArray.buffer && UTF8Decoder) {
    return UTF8Decoder.decode(heapOrArray.subarray(idx, endPtr));
  }
  var str = '';
  // If building with TextDecoder, we have already computed the string length
  // above, so test loop end condition against that
  while (idx < endPtr) {
    // For UTF8 byte structure, see:
    // http://en.wikipedia.org/wiki/UTF-8#Description
    // https://www.ietf.org/rfc/rfc2279.txt
    // https://tools.ietf.org/html/rfc3629
    var u0 = heapOrArray[idx++];
    if (!(u0 & 0x80)) {
      str += String.fromCharCode(u0);
      continue;
    }
    var u1 = heapOrArray[idx++] & 63;
    if ((u0 & 0xE0) == 0xC0) {
      str += String.fromCharCode((u0 & 31) << 6 | u1);
      continue;
    }
    var u2 = heapOrArray[idx++] & 63;
    if ((u0 & 0xF0) == 0xE0) {
      u0 = (u0 & 15) << 12 | u1 << 6 | u2;
    } else {
      u0 = (u0 & 7) << 18 | u1 << 12 | u2 << 6 | heapOrArray[idx++] & 63;
    }
    if (u0 < 0x10000) {
      str += String.fromCharCode(u0);
    } else {
      var ch = u0 - 0x10000;
      str += String.fromCharCode(0xD800 | ch >> 10, 0xDC00 | ch & 0x3FF);
    }
  }
  return str;
};
var FS_stdin_getChar_buffer = [];
var lengthBytesUTF8 = function lengthBytesUTF8(str) {
  var len = 0;
  for (var i = 0; i < str.length; ++i) {
    // Gotcha: charCodeAt returns a 16-bit word that is a UTF-16 encoded code
    // unit, not a Unicode code point of the character! So decode
    // UTF16->UTF32->UTF8.
    // See http://unicode.org/faq/utf_bom.html#utf16-3
    var c = str.charCodeAt(i); // possibly a lead surrogate
    if (c <= 0x7F) {
      len++;
    } else if (c <= 0x7FF) {
      len += 2;
    } else if (c >= 0xD800 && c <= 0xDFFF) {
      len += 4;
      ++i;
    } else {
      len += 3;
    }
  }
  return len;
};
var stringToUTF8Array = function stringToUTF8Array(str, heap, outIdx, maxBytesToWrite) {
  // Parameter maxBytesToWrite is not optional. Negative values, 0, null,
  // undefined and false each don't write out any bytes.
  if (!(maxBytesToWrite > 0)) return 0;
  var startIdx = outIdx;
  var endIdx = outIdx + maxBytesToWrite - 1; // -1 for string null terminator.
  for (var i = 0; i < str.length; ++i) {
    // Gotcha: charCodeAt returns a 16-bit word that is a UTF-16 encoded code
    // unit, not a Unicode code point of the character! So decode
    // UTF16->UTF32->UTF8.
    // See http://unicode.org/faq/utf_bom.html#utf16-3
    // For UTF8 byte structure, see http://en.wikipedia.org/wiki/UTF-8#Description
    // and https://www.ietf.org/rfc/rfc2279.txt
    // and https://tools.ietf.org/html/rfc3629
    var u = str.charCodeAt(i); // possibly a lead surrogate
    if (u >= 0xD800 && u <= 0xDFFF) {
      var u1 = str.charCodeAt(++i);
      u = 0x10000 + ((u & 0x3FF) << 10) | u1 & 0x3FF;
    }
    if (u <= 0x7F) {
      if (outIdx >= endIdx) break;
      heap[outIdx++] = u;
    } else if (u <= 0x7FF) {
      if (outIdx + 1 >= endIdx) break;
      heap[outIdx++] = 0xC0 | u >> 6;
      heap[outIdx++] = 0x80 | u & 63;
    } else if (u <= 0xFFFF) {
      if (outIdx + 2 >= endIdx) break;
      heap[outIdx++] = 0xE0 | u >> 12;
      heap[outIdx++] = 0x80 | u >> 6 & 63;
      heap[outIdx++] = 0x80 | u & 63;
    } else {
      if (outIdx + 3 >= endIdx) break;
      heap[outIdx++] = 0xF0 | u >> 18;
      heap[outIdx++] = 0x80 | u >> 12 & 63;
      heap[outIdx++] = 0x80 | u >> 6 & 63;
      heap[outIdx++] = 0x80 | u & 63;
    }
  }
  // Null-terminate the pointer to the buffer.
  heap[outIdx] = 0;
  return outIdx - startIdx;
};
/** @type {function(string, boolean=, number=)} */
var intArrayFromString = function intArrayFromString(stringy, dontAddNull, length) {
  var len = length > 0 ? length : lengthBytesUTF8(stringy) + 1;
  var u8array = new Array(len);
  var numBytesWritten = stringToUTF8Array(stringy, u8array, 0, u8array.length);
  if (dontAddNull) u8array.length = numBytesWritten;
  return u8array;
};
var FS_stdin_getChar = function FS_stdin_getChar() {
  if (!FS_stdin_getChar_buffer.length) {
    var result = null;
    if (ENVIRONMENT_IS_NODE) {
      // we will read data by chunks of BUFSIZE
      var BUFSIZE = 256;
      var buf = Buffer.alloc(BUFSIZE);
      var bytesRead = 0;

      // For some reason we must suppress a closure warning here, even though
      // fd definitely exists on process.stdin, and is even the proper way to
      // get the fd of stdin,
      // https://github.com/nodejs/help/issues/2136#issuecomment-523649904
      // This started to happen after moving this logic out of library_tty.js,
      // so it is related to the surrounding code in some unclear manner.
      /** @suppress {missingProperties} */
      var fd = process.stdin.fd;
      try {
        bytesRead = fs.readSync(fd, buf, 0, BUFSIZE);
      } catch (e) {
        // Cross-platform differences: on Windows, reading EOF throws an
        // exception, but on other OSes, reading EOF returns 0. Uniformize
        // behavior by treating the EOF exception to return 0.
        if (e.toString().includes('EOF')) bytesRead = 0;else throw e;
      }
      if (bytesRead > 0) {
        result = buf.slice(0, bytesRead).toString('utf-8');
      }
    } else if (typeof window != 'undefined' && typeof window.prompt == 'function') {
      // Browser.
      result = window.prompt('Input: '); // returns null on cancel
      if (result !== null) {
        result += '\n';
      }
    } else {}
    if (!result) {
      return null;
    }
    FS_stdin_getChar_buffer = intArrayFromString(result, true);
  }
  return FS_stdin_getChar_buffer.shift();
};
var TTY = {
  ttys: [],
  init: function init() {
    // https://github.com/emscripten-core/emscripten/pull/1555
    // if (ENVIRONMENT_IS_NODE) {
    //   // currently, FS.init does not distinguish if process.stdin is a file or TTY
    //   // device, it always assumes it's a TTY device. because of this, we're forcing
    //   // process.stdin to UTF8 encoding to at least make stdin reading compatible
    //   // with text files until FS.init can be refactored.
    //   process.stdin.setEncoding('utf8');
    // }
  },
  shutdown: function shutdown() {
    // https://github.com/emscripten-core/emscripten/pull/1555
    // if (ENVIRONMENT_IS_NODE) {
    //   // inolen: any idea as to why node -e 'process.stdin.read()' wouldn't exit immediately (with process.stdin being a tty)?
    //   // isaacs: because now it's reading from the stream, you've expressed interest in it, so that read() kicks off a _read() which creates a ReadReq operation
    //   // inolen: I thought read() in that case was a synchronous operation that just grabbed some amount of buffered data if it exists?
    //   // isaacs: it is. but it also triggers a _read() call, which calls readStart() on the handle
    //   // isaacs: do process.stdin.pause() and i'd think it'd probably close the pending call
    //   process.stdin.pause();
    // }
  },
  register: function register(dev, ops) {
    TTY.ttys[dev] = {
      input: [],
      output: [],
      ops: ops
    };
    FS.registerDevice(dev, TTY.stream_ops);
  },
  stream_ops: {
    open: function open(stream) {
      var tty = TTY.ttys[stream.node.rdev];
      if (!tty) {
        throw new FS.ErrnoError(43);
      }
      stream.tty = tty;
      stream.seekable = false;
    },
    close: function close(stream) {
      // flush any pending line data
      stream.tty.ops.fsync(stream.tty);
    },
    fsync: function fsync(stream) {
      stream.tty.ops.fsync(stream.tty);
    },
    read: function read(stream, buffer, offset, length, pos /* ignored */) {
      if (!stream.tty || !stream.tty.ops.get_char) {
        throw new FS.ErrnoError(60);
      }
      var bytesRead = 0;
      for (var i = 0; i < length; i++) {
        var result;
        try {
          result = stream.tty.ops.get_char(stream.tty);
        } catch (e) {
          throw new FS.ErrnoError(29);
        }
        if (result === undefined && bytesRead === 0) {
          throw new FS.ErrnoError(6);
        }
        if (result === null || result === undefined) break;
        bytesRead++;
        buffer[offset + i] = result;
      }
      if (bytesRead) {
        stream.node.atime = Date.now();
      }
      return bytesRead;
    },
    write: function write(stream, buffer, offset, length, pos) {
      if (!stream.tty || !stream.tty.ops.put_char) {
        throw new FS.ErrnoError(60);
      }
      try {
        for (var i = 0; i < length; i++) {
          stream.tty.ops.put_char(stream.tty, buffer[offset + i]);
        }
      } catch (e) {
        throw new FS.ErrnoError(29);
      }
      if (length) {
        stream.node.mtime = stream.node.ctime = Date.now();
      }
      return i;
    }
  },
  default_tty_ops: {
    get_char: function get_char(tty) {
      return FS_stdin_getChar();
    },
    put_char: function put_char(tty, val) {
      if (val === null || val === 10) {
        out(UTF8ArrayToString(tty.output));
        tty.output = [];
      } else {
        if (val != 0) tty.output.push(val); // val == 0 would cut text output off in the middle.
      }
    },
    fsync: function fsync(tty) {
      var _tty$output;
      if (((_tty$output = tty.output) === null || _tty$output === void 0 ? void 0 : _tty$output.length) > 0) {
        out(UTF8ArrayToString(tty.output));
        tty.output = [];
      }
    },
    ioctl_tcgets: function ioctl_tcgets(tty) {
      // typical setting
      return {
        c_iflag: 25856,
        c_oflag: 5,
        c_cflag: 191,
        c_lflag: 35387,
        c_cc: [0x03, 0x1c, 0x7f, 0x15, 0x04, 0x00, 0x01, 0x00, 0x11, 0x13, 0x1a, 0x00, 0x12, 0x0f, 0x17, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]
      };
    },
    ioctl_tcsets: function ioctl_tcsets(tty, optional_actions, data) {
      // currently just ignore
      return 0;
    },
    ioctl_tiocgwinsz: function ioctl_tiocgwinsz(tty) {
      return [24, 80];
    }
  },
  default_tty1_ops: {
    put_char: function put_char(tty, val) {
      if (val === null || val === 10) {
        err(UTF8ArrayToString(tty.output));
        tty.output = [];
      } else {
        if (val != 0) tty.output.push(val);
      }
    },
    fsync: function fsync(tty) {
      var _tty$output2;
      if (((_tty$output2 = tty.output) === null || _tty$output2 === void 0 ? void 0 : _tty$output2.length) > 0) {
        err(UTF8ArrayToString(tty.output));
        tty.output = [];
      }
    }
  }
};
var mmapAlloc = function mmapAlloc(size) {
  abort();
};
var MEMFS = {
  ops_table: null,
  mount: function mount(_mount) {
    return MEMFS.createNode(null, '/', 16895, 0);
  },
  createNode: function createNode(parent, name, mode, dev) {
    if (FS.isBlkdev(mode) || FS.isFIFO(mode)) {
      // no supported
      throw new FS.ErrnoError(63);
    }
    MEMFS.ops_table || (MEMFS.ops_table = {
      dir: {
        node: {
          getattr: MEMFS.node_ops.getattr,
          setattr: MEMFS.node_ops.setattr,
          lookup: MEMFS.node_ops.lookup,
          mknod: MEMFS.node_ops.mknod,
          rename: MEMFS.node_ops.rename,
          unlink: MEMFS.node_ops.unlink,
          rmdir: MEMFS.node_ops.rmdir,
          readdir: MEMFS.node_ops.readdir,
          symlink: MEMFS.node_ops.symlink
        },
        stream: {
          llseek: MEMFS.stream_ops.llseek
        }
      },
      file: {
        node: {
          getattr: MEMFS.node_ops.getattr,
          setattr: MEMFS.node_ops.setattr
        },
        stream: {
          llseek: MEMFS.stream_ops.llseek,
          read: MEMFS.stream_ops.read,
          write: MEMFS.stream_ops.write,
          mmap: MEMFS.stream_ops.mmap,
          msync: MEMFS.stream_ops.msync
        }
      },
      link: {
        node: {
          getattr: MEMFS.node_ops.getattr,
          setattr: MEMFS.node_ops.setattr,
          readlink: MEMFS.node_ops.readlink
        },
        stream: {}
      },
      chrdev: {
        node: {
          getattr: MEMFS.node_ops.getattr,
          setattr: MEMFS.node_ops.setattr
        },
        stream: FS.chrdev_stream_ops
      }
    });
    var node = FS.createNode(parent, name, mode, dev);
    if (FS.isDir(node.mode)) {
      node.node_ops = MEMFS.ops_table.dir.node;
      node.stream_ops = MEMFS.ops_table.dir.stream;
      node.contents = {};
    } else if (FS.isFile(node.mode)) {
      node.node_ops = MEMFS.ops_table.file.node;
      node.stream_ops = MEMFS.ops_table.file.stream;
      node.usedBytes = 0; // The actual number of bytes used in the typed array, as opposed to contents.length which gives the whole capacity.
      // When the byte data of the file is populated, this will point to either a typed array, or a normal JS array. Typed arrays are preferred
      // for performance, and used by default. However, typed arrays are not resizable like normal JS arrays are, so there is a small disk size
      // penalty involved for appending file writes that continuously grow a file similar to std::vector capacity vs used -scheme.
      node.contents = null;
    } else if (FS.isLink(node.mode)) {
      node.node_ops = MEMFS.ops_table.link.node;
      node.stream_ops = MEMFS.ops_table.link.stream;
    } else if (FS.isChrdev(node.mode)) {
      node.node_ops = MEMFS.ops_table.chrdev.node;
      node.stream_ops = MEMFS.ops_table.chrdev.stream;
    }
    node.atime = node.mtime = node.ctime = Date.now();
    // add the new node to the parent
    if (parent) {
      parent.contents[name] = node;
      parent.atime = parent.mtime = parent.ctime = node.atime;
    }
    return node;
  },
  getFileDataAsTypedArray: function getFileDataAsTypedArray(node) {
    if (!node.contents) return new Uint8Array(0);
    if (node.contents.subarray) return node.contents.subarray(0, node.usedBytes); // Make sure to not return excess unused bytes.
    return new Uint8Array(node.contents);
  },
  expandFileStorage: function expandFileStorage(node, newCapacity) {
    var prevCapacity = node.contents ? node.contents.length : 0;
    if (prevCapacity >= newCapacity) return; // No need to expand, the storage was already large enough.
    // Don't expand strictly to the given requested limit if it's only a very small increase, but instead geometrically grow capacity.
    // For small filesizes (<1MB), perform size*2 geometric increase, but for large sizes, do a much more conservative size*1.125 increase to
    // avoid overshooting the allocation cap by a very large margin.
    var CAPACITY_DOUBLING_MAX = 1024 * 1024;
    newCapacity = Math.max(newCapacity, prevCapacity * (prevCapacity < CAPACITY_DOUBLING_MAX ? 2.0 : 1.125) >>> 0);
    if (prevCapacity != 0) newCapacity = Math.max(newCapacity, 256); // At minimum allocate 256b for each file when expanding.
    var oldContents = node.contents;
    node.contents = new Uint8Array(newCapacity); // Allocate new storage.
    if (node.usedBytes > 0) node.contents.set(oldContents.subarray(0, node.usedBytes), 0); // Copy old data over to the new storage.
  },
  resizeFileStorage: function resizeFileStorage(node, newSize) {
    if (node.usedBytes == newSize) return;
    if (newSize == 0) {
      node.contents = null; // Fully decommit when requesting a resize to zero.
      node.usedBytes = 0;
    } else {
      var oldContents = node.contents;
      node.contents = new Uint8Array(newSize); // Allocate new storage.
      if (oldContents) {
        node.contents.set(oldContents.subarray(0, Math.min(newSize, node.usedBytes))); // Copy old data over to the new storage.
      }
      node.usedBytes = newSize;
    }
  },
  node_ops: {
    getattr: function getattr(node) {
      var attr = {};
      // device numbers reuse inode numbers.
      attr.dev = FS.isChrdev(node.mode) ? node.id : 1;
      attr.ino = node.id;
      attr.mode = node.mode;
      attr.nlink = 1;
      attr.uid = 0;
      attr.gid = 0;
      attr.rdev = node.rdev;
      if (FS.isDir(node.mode)) {
        attr.size = 4096;
      } else if (FS.isFile(node.mode)) {
        attr.size = node.usedBytes;
      } else if (FS.isLink(node.mode)) {
        attr.size = node.link.length;
      } else {
        attr.size = 0;
      }
      attr.atime = new Date(node.atime);
      attr.mtime = new Date(node.mtime);
      attr.ctime = new Date(node.ctime);
      // NOTE: In our implementation, st_blocks = Math.ceil(st_size/st_blksize),
      //       but this is not required by the standard.
      attr.blksize = 4096;
      attr.blocks = Math.ceil(attr.size / attr.blksize);
      return attr;
    },
    setattr: function setattr(node, attr) {
      for (var _i = 0, _arr = ["mode", "atime", "mtime", "ctime"]; _i < _arr.length; _i++) {
        var key = _arr[_i];
        if (attr[key] != null) {
          node[key] = attr[key];
        }
      }
      if (attr.size !== undefined) {
        MEMFS.resizeFileStorage(node, attr.size);
      }
    },
    lookup: function lookup(parent, name) {
      throw MEMFS.doesNotExistError;
    },
    mknod: function mknod(parent, name, mode, dev) {
      return MEMFS.createNode(parent, name, mode, dev);
    },
    rename: function rename(old_node, new_dir, new_name) {
      var new_node;
      try {
        new_node = FS.lookupNode(new_dir, new_name);
      } catch (e) {}
      if (new_node) {
        if (FS.isDir(old_node.mode)) {
          // if we're overwriting a directory at new_name, make sure it's empty.
          for (var i in new_node.contents) {
            throw new FS.ErrnoError(55);
          }
        }
        FS.hashRemoveNode(new_node);
      }
      // do the internal rewiring
      delete old_node.parent.contents[old_node.name];
      new_dir.contents[new_name] = old_node;
      old_node.name = new_name;
      new_dir.ctime = new_dir.mtime = old_node.parent.ctime = old_node.parent.mtime = Date.now();
    },
    unlink: function unlink(parent, name) {
      delete parent.contents[name];
      parent.ctime = parent.mtime = Date.now();
    },
    rmdir: function rmdir(parent, name) {
      var node = FS.lookupNode(parent, name);
      for (var i in node.contents) {
        throw new FS.ErrnoError(55);
      }
      delete parent.contents[name];
      parent.ctime = parent.mtime = Date.now();
    },
    readdir: function readdir(node) {
      return ['.', '..'].concat(_toConsumableArray(Object.keys(node.contents)));
    },
    symlink: function symlink(parent, newname, oldpath) {
      var node = MEMFS.createNode(parent, newname, 511 | 40960, 0);
      node.link = oldpath;
      return node;
    },
    readlink: function readlink(node) {
      if (!FS.isLink(node.mode)) {
        throw new FS.ErrnoError(28);
      }
      return node.link;
    }
  },
  stream_ops: {
    read: function read(stream, buffer, offset, length, position) {
      var contents = stream.node.contents;
      if (position >= stream.node.usedBytes) return 0;
      var size = Math.min(stream.node.usedBytes - position, length);
      if (size > 8 && contents.subarray) {
        // non-trivial, and typed array
        buffer.set(contents.subarray(position, position + size), offset);
      } else {
        for (var i = 0; i < size; i++) buffer[offset + i] = contents[position + i];
      }
      return size;
    },
    write: function write(stream, buffer, offset, length, position, canOwn) {
      if (!length) return 0;
      var node = stream.node;
      node.mtime = node.ctime = Date.now();
      if (buffer.subarray && (!node.contents || node.contents.subarray)) {
        // This write is from a typed array to a typed array?
        if (canOwn) {
          node.contents = buffer.subarray(offset, offset + length);
          node.usedBytes = length;
          return length;
        } else if (node.usedBytes === 0 && position === 0) {
          // If this is a simple first write to an empty file, do a fast set since we don't need to care about old data.
          node.contents = buffer.slice(offset, offset + length);
          node.usedBytes = length;
          return length;
        } else if (position + length <= node.usedBytes) {
          // Writing to an already allocated and used subrange of the file?
          node.contents.set(buffer.subarray(offset, offset + length), position);
          return length;
        }
      }

      // Appending to an existing file and we need to reallocate, or source data did not come as a typed array.
      MEMFS.expandFileStorage(node, position + length);
      if (node.contents.subarray && buffer.subarray) {
        // Use typed array write which is available.
        node.contents.set(buffer.subarray(offset, offset + length), position);
      } else {
        for (var i = 0; i < length; i++) {
          node.contents[position + i] = buffer[offset + i]; // Or fall back to manual write if not.
        }
      }
      node.usedBytes = Math.max(node.usedBytes, position + length);
      return length;
    },
    llseek: function llseek(stream, offset, whence) {
      var position = offset;
      if (whence === 1) {
        position += stream.position;
      } else if (whence === 2) {
        if (FS.isFile(stream.node.mode)) {
          position += stream.node.usedBytes;
        }
      }
      if (position < 0) {
        throw new FS.ErrnoError(28);
      }
      return position;
    },
    mmap: function mmap(stream, length, position, prot, flags) {
      if (!FS.isFile(stream.node.mode)) {
        throw new FS.ErrnoError(43);
      }
      var ptr;
      var allocated;
      var contents = stream.node.contents;
      // Only make a new copy when MAP_PRIVATE is specified.
      if (!(flags & 2) && contents && contents.buffer === HEAP8.buffer) {
        // We can't emulate MAP_SHARED when the file is not backed by the
        // buffer we're mapping to (e.g. the HEAP buffer).
        allocated = false;
        ptr = contents.byteOffset;
      } else {
        allocated = true;
        ptr = mmapAlloc(length);
        if (!ptr) {
          throw new FS.ErrnoError(48);
        }
        if (contents) {
          // Try to avoid unnecessary slices.
          if (position > 0 || position + length < contents.length) {
            if (contents.subarray) {
              contents = contents.subarray(position, position + length);
            } else {
              contents = Array.prototype.slice.call(contents, position, position + length);
            }
          }
          HEAP8.set(contents, ptr);
        }
      }
      return {
        ptr: ptr,
        allocated: allocated
      };
    },
    msync: function msync(stream, buffer, offset, length, mmapFlags) {
      MEMFS.stream_ops.write(stream, buffer, 0, length, offset, false);
      // should we check if bytesWritten and length are the same?
      return 0;
    }
  }
};
var asyncLoad = /*#__PURE__*/function () {
  var _ref3 = _asyncToGenerator(/*#__PURE__*/_regeneratorRuntime().mark(function _callee3(url) {
    var arrayBuffer;
    return _regeneratorRuntime().wrap(function _callee3$(_context3) {
      while (1) switch (_context3.prev = _context3.next) {
        case 0:
          _context3.next = 2;
          return readAsync(url);
        case 2:
          arrayBuffer = _context3.sent;
          return _context3.abrupt("return", new Uint8Array(arrayBuffer));
        case 4:
        case "end":
          return _context3.stop();
      }
    }, _callee3);
  }));
  return function asyncLoad(_x10) {
    return _ref3.apply(this, arguments);
  };
}();
asyncLoad.isAsync = true;
var FS_createDataFile = function FS_createDataFile(parent, name, fileData, canRead, canWrite, canOwn) {
  FS.createDataFile(parent, name, fileData, canRead, canWrite, canOwn);
};
var preloadPlugins = Module['preloadPlugins'] || [];
var FS_handledByPreloadPlugin = function FS_handledByPreloadPlugin(byteArray, fullname, finish, onerror) {
  // Ensure plugins are ready.
  if (typeof Browser != 'undefined') Browser.init();
  var handled = false;
  preloadPlugins.forEach(function (plugin) {
    if (handled) return;
    if (plugin['canHandle'](fullname)) {
      plugin['handle'](byteArray, fullname, finish, onerror);
      handled = true;
    }
  });
  return handled;
};
var FS_createPreloadedFile = function FS_createPreloadedFile(parent, name, url, canRead, canWrite, onload, onerror, dontCreateFile, canOwn, preFinish) {
  // TODO we should allow people to just pass in a complete filename instead
  // of parent and name being that we just join them anyways
  var fullname = name ? PATH_FS.resolve(PATH.join2(parent, name)) : parent;
  var dep = getUniqueRunDependency("cp ".concat(fullname)); // might have several active requests for the same fullname
  function processData(byteArray) {
    function finish(byteArray) {
      preFinish === null || preFinish === void 0 || preFinish();
      if (!dontCreateFile) {
        FS_createDataFile(parent, name, byteArray, canRead, canWrite, canOwn);
      }
      onload === null || onload === void 0 || onload();
      removeRunDependency(dep);
    }
    if (FS_handledByPreloadPlugin(byteArray, fullname, finish, function () {
      onerror === null || onerror === void 0 || onerror();
      removeRunDependency(dep);
    })) {
      return;
    }
    finish(byteArray);
  }
  addRunDependency(dep);
  if (typeof url == 'string') {
    asyncLoad(url).then(processData, onerror);
  } else {
    processData(url);
  }
};
var FS_modeStringToFlags = function FS_modeStringToFlags(str) {
  var flagModes = {
    'r': 0,
    'r+': 2,
    'w': 512 | 64 | 1,
    'w+': 512 | 64 | 2,
    'a': 1024 | 64 | 1,
    'a+': 1024 | 64 | 2
  };
  var flags = flagModes[str];
  if (typeof flags == 'undefined') {
    throw new Error("Unknown file open mode: ".concat(str));
  }
  return flags;
};
var FS_getMode = function FS_getMode(canRead, canWrite) {
  var mode = 0;
  if (canRead) mode |= 292 | 73;
  if (canWrite) mode |= 146;
  return mode;
};
var FS = {
  root: null,
  mounts: [],
  devices: {},
  streams: [],
  nextInode: 1,
  nameTable: null,
  currentPath: "/",
  initialized: false,
  ignorePermissions: true,
  filesystems: null,
  syncFSRequests: 0,
  readFiles: {},
  ErrnoError: /*#__PURE__*/_createClass(
  // We set the `name` property to be able to identify `FS.ErrnoError`
  // - the `name` is a standard ECMA-262 property of error objects. Kind of good to have it anyway.
  // - when using PROXYFS, an error can come from an underlying FS
  // as different FS objects have their own FS.ErrnoError each,
  // the test `err instanceof FS.ErrnoError` won't detect an error coming from another filesystem, causing bugs.
  // we'll use the reliable test `err.name == "ErrnoError"` instead
  function ErrnoError(errno) {
    "use strict";

    _classCallCheck(this, ErrnoError);
    _defineProperty(this, "name", 'ErrnoError');
    this.errno = errno;
  }),
  FSStream: /*#__PURE__*/function () {
    "use strict";

    function FSStream() {
      _classCallCheck(this, FSStream);
      _defineProperty(this, "shared", {});
    }
    return _createClass(FSStream, [{
      key: "object",
      get: function get() {
        return this.node;
      },
      set: function set(val) {
        this.node = val;
      }
    }, {
      key: "isRead",
      get: function get() {
        return (this.flags & 2097155) !== 1;
      }
    }, {
      key: "isWrite",
      get: function get() {
        return (this.flags & 2097155) !== 0;
      }
    }, {
      key: "isAppend",
      get: function get() {
        return this.flags & 1024;
      }
    }, {
      key: "flags",
      get: function get() {
        return this.shared.flags;
      },
      set: function set(val) {
        this.shared.flags = val;
      }
    }, {
      key: "position",
      get: function get() {
        return this.shared.position;
      },
      set: function set(val) {
        this.shared.position = val;
      }
    }]);
  }(),
  FSNode: /*#__PURE__*/function () {
    "use strict";

    function FSNode(parent, name, mode, rdev) {
      _classCallCheck(this, FSNode);
      _defineProperty(this, "node_ops", {});
      _defineProperty(this, "stream_ops", {});
      _defineProperty(this, "readMode", 292 | 73);
      _defineProperty(this, "writeMode", 146);
      _defineProperty(this, "mounted", null);
      if (!parent) {
        parent = this; // root node sets parent to itself
      }
      this.parent = parent;
      this.mount = parent.mount;
      this.id = FS.nextInode++;
      this.name = name;
      this.mode = mode;
      this.rdev = rdev;
      this.atime = this.mtime = this.ctime = Date.now();
    }
    return _createClass(FSNode, [{
      key: "read",
      get: function get() {
        return (this.mode & this.readMode) === this.readMode;
      },
      set: function set(val) {
        val ? this.mode |= this.readMode : this.mode &= ~this.readMode;
      }
    }, {
      key: "write",
      get: function get() {
        return (this.mode & this.writeMode) === this.writeMode;
      },
      set: function set(val) {
        val ? this.mode |= this.writeMode : this.mode &= ~this.writeMode;
      }
    }, {
      key: "isFolder",
      get: function get() {
        return FS.isDir(this.mode);
      }
    }, {
      key: "isDevice",
      get: function get() {
        return FS.isChrdev(this.mode);
      }
    }]);
  }(),
  lookupPath: function lookupPath(path) {
    var _opts$follow_mount;
    var opts = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : {};
    if (!path) {
      throw new FS.ErrnoError(44);
    }
    (_opts$follow_mount = opts.follow_mount) !== null && _opts$follow_mount !== void 0 ? _opts$follow_mount : opts.follow_mount = true;
    if (!PATH.isAbs(path)) {
      path = FS.cwd() + '/' + path;
    }

    // limit max consecutive symlinks to 40 (SYMLOOP_MAX).
    linkloop: for (var nlinks = 0; nlinks < 40; nlinks++) {
      // split the absolute path
      var parts = path.split('/').filter(function (p) {
        return !!p;
      });

      // start at the root
      var current = FS.root;
      var current_path = '/';
      for (var i = 0; i < parts.length; i++) {
        var islast = i === parts.length - 1;
        if (islast && opts.parent) {
          // stop resolving
          break;
        }
        if (parts[i] === '.') {
          continue;
        }
        if (parts[i] === '..') {
          current_path = PATH.dirname(current_path);
          current = current.parent;
          continue;
        }
        current_path = PATH.join2(current_path, parts[i]);
        try {
          current = FS.lookupNode(current, parts[i]);
        } catch (e) {
          // if noent_okay is true, suppress a ENOENT in the last component
          // and return an object with an undefined node. This is needed for
          // resolving symlinks in the path when creating a file.
          if ((e === null || e === void 0 ? void 0 : e.errno) === 44 && islast && opts.noent_okay) {
            return {
              path: current_path
            };
          }
          throw e;
        }

        // jump to the mount's root node if this is a mountpoint
        if (FS.isMountpoint(current) && (!islast || opts.follow_mount)) {
          current = current.mounted.root;
        }

        // by default, lookupPath will not follow a symlink if it is the final path component.
        // setting opts.follow = true will override this behavior.
        if (FS.isLink(current.mode) && (!islast || opts.follow)) {
          if (!current.node_ops.readlink) {
            throw new FS.ErrnoError(52);
          }
          var link = current.node_ops.readlink(current);
          if (!PATH.isAbs(link)) {
            link = PATH.dirname(current_path) + '/' + link;
          }
          path = link + '/' + parts.slice(i + 1).join('/');
          continue linkloop;
        }
      }
      return {
        path: current_path,
        node: current
      };
    }
    throw new FS.ErrnoError(32);
  },
  getPath: function getPath(node) {
    var path;
    while (true) {
      if (FS.isRoot(node)) {
        var mount = node.mount.mountpoint;
        if (!path) return mount;
        return mount[mount.length - 1] !== '/' ? "".concat(mount, "/").concat(path) : mount + path;
      }
      path = path ? "".concat(node.name, "/").concat(path) : node.name;
      node = node.parent;
    }
  },
  hashName: function hashName(parentid, name) {
    var hash = 0;
    for (var i = 0; i < name.length; i++) {
      hash = (hash << 5) - hash + name.charCodeAt(i) | 0;
    }
    return (parentid + hash >>> 0) % FS.nameTable.length;
  },
  hashAddNode: function hashAddNode(node) {
    var hash = FS.hashName(node.parent.id, node.name);
    node.name_next = FS.nameTable[hash];
    FS.nameTable[hash] = node;
  },
  hashRemoveNode: function hashRemoveNode(node) {
    var hash = FS.hashName(node.parent.id, node.name);
    if (FS.nameTable[hash] === node) {
      FS.nameTable[hash] = node.name_next;
    } else {
      var current = FS.nameTable[hash];
      while (current) {
        if (current.name_next === node) {
          current.name_next = node.name_next;
          break;
        }
        current = current.name_next;
      }
    }
  },
  lookupNode: function lookupNode(parent, name) {
    var errCode = FS.mayLookup(parent);
    if (errCode) {
      throw new FS.ErrnoError(errCode);
    }
    var hash = FS.hashName(parent.id, name);
    for (var node = FS.nameTable[hash]; node; node = node.name_next) {
      var nodeName = node.name;
      if (node.parent.id === parent.id && nodeName === name) {
        return node;
      }
    }
    // if we failed to find it in the cache, call into the VFS
    return FS.lookup(parent, name);
  },
  createNode: function createNode(parent, name, mode, rdev) {
    var node = new FS.FSNode(parent, name, mode, rdev);
    FS.hashAddNode(node);
    return node;
  },
  destroyNode: function destroyNode(node) {
    FS.hashRemoveNode(node);
  },
  isRoot: function isRoot(node) {
    return node === node.parent;
  },
  isMountpoint: function isMountpoint(node) {
    return !!node.mounted;
  },
  isFile: function isFile(mode) {
    return (mode & 61440) === 32768;
  },
  isDir: function isDir(mode) {
    return (mode & 61440) === 16384;
  },
  isLink: function isLink(mode) {
    return (mode & 61440) === 40960;
  },
  isChrdev: function isChrdev(mode) {
    return (mode & 61440) === 8192;
  },
  isBlkdev: function isBlkdev(mode) {
    return (mode & 61440) === 24576;
  },
  isFIFO: function isFIFO(mode) {
    return (mode & 61440) === 4096;
  },
  isSocket: function isSocket(mode) {
    return (mode & 49152) === 49152;
  },
  flagsToPermissionString: function flagsToPermissionString(flag) {
    var perms = ['r', 'w', 'rw'][flag & 3];
    if (flag & 512) {
      perms += 'w';
    }
    return perms;
  },
  nodePermissions: function nodePermissions(node, perms) {
    if (FS.ignorePermissions) {
      return 0;
    }
    // return 0 if any user, group or owner bits are set.
    if (perms.includes('r') && !(node.mode & 292)) {
      return 2;
    } else if (perms.includes('w') && !(node.mode & 146)) {
      return 2;
    } else if (perms.includes('x') && !(node.mode & 73)) {
      return 2;
    }
    return 0;
  },
  mayLookup: function mayLookup(dir) {
    if (!FS.isDir(dir.mode)) return 54;
    var errCode = FS.nodePermissions(dir, 'x');
    if (errCode) return errCode;
    if (!dir.node_ops.lookup) return 2;
    return 0;
  },
  mayCreate: function mayCreate(dir, name) {
    if (!FS.isDir(dir.mode)) {
      return 54;
    }
    try {
      var node = FS.lookupNode(dir, name);
      return 20;
    } catch (e) {}
    return FS.nodePermissions(dir, 'wx');
  },
  mayDelete: function mayDelete(dir, name, isdir) {
    var node;
    try {
      node = FS.lookupNode(dir, name);
    } catch (e) {
      return e.errno;
    }
    var errCode = FS.nodePermissions(dir, 'wx');
    if (errCode) {
      return errCode;
    }
    if (isdir) {
      if (!FS.isDir(node.mode)) {
        return 54;
      }
      if (FS.isRoot(node) || FS.getPath(node) === FS.cwd()) {
        return 10;
      }
    } else {
      if (FS.isDir(node.mode)) {
        return 31;
      }
    }
    return 0;
  },
  mayOpen: function mayOpen(node, flags) {
    if (!node) {
      return 44;
    }
    if (FS.isLink(node.mode)) {
      return 32;
    } else if (FS.isDir(node.mode)) {
      if (FS.flagsToPermissionString(flags) !== 'r' // opening for write
      || flags & (512 | 64)) {
        // TODO: check for O_SEARCH? (== search for dir only)
        return 31;
      }
    }
    return FS.nodePermissions(node, FS.flagsToPermissionString(flags));
  },
  checkOpExists: function checkOpExists(op, err) {
    if (!op) {
      throw new FS.ErrnoError(err);
    }
    return op;
  },
  MAX_OPEN_FDS: 4096,
  nextfd: function nextfd() {
    for (var fd = 0; fd <= FS.MAX_OPEN_FDS; fd++) {
      if (!FS.streams[fd]) {
        return fd;
      }
    }
    throw new FS.ErrnoError(33);
  },
  getStreamChecked: function getStreamChecked(fd) {
    var stream = FS.getStream(fd);
    if (!stream) {
      throw new FS.ErrnoError(8);
    }
    return stream;
  },
  getStream: function getStream(fd) {
    return FS.streams[fd];
  },
  createStream: function createStream(stream) {
    var fd = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : -1;
    // clone it, so we can return an instance of FSStream
    stream = Object.assign(new FS.FSStream(), stream);
    if (fd == -1) {
      fd = FS.nextfd();
    }
    stream.fd = fd;
    FS.streams[fd] = stream;
    return stream;
  },
  closeStream: function closeStream(fd) {
    FS.streams[fd] = null;
  },
  dupStream: function dupStream(origStream) {
    var _stream$stream_ops, _stream$stream_ops$du;
    var fd = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : -1;
    var stream = FS.createStream(origStream, fd);
    (_stream$stream_ops = stream.stream_ops) === null || _stream$stream_ops === void 0 || (_stream$stream_ops$du = _stream$stream_ops.dup) === null || _stream$stream_ops$du === void 0 || _stream$stream_ops$du.call(_stream$stream_ops, stream);
    return stream;
  },
  doSetAttr: function doSetAttr(stream, node, attr) {
    var setattr = stream === null || stream === void 0 ? void 0 : stream.stream_ops.setattr;
    var arg = setattr ? stream : node;
    setattr !== null && setattr !== void 0 ? setattr : setattr = node.node_ops.setattr;
    FS.checkOpExists(setattr, 63);
    setattr(arg, attr);
  },
  chrdev_stream_ops: {
    open: function open(stream) {
      var _stream$stream_ops$op, _stream$stream_ops2;
      var device = FS.getDevice(stream.node.rdev);
      // override node's stream ops with the device's
      stream.stream_ops = device.stream_ops;
      // forward the open call
      (_stream$stream_ops$op = (_stream$stream_ops2 = stream.stream_ops).open) === null || _stream$stream_ops$op === void 0 || _stream$stream_ops$op.call(_stream$stream_ops2, stream);
    },
    llseek: function llseek() {
      throw new FS.ErrnoError(70);
    }
  },
  major: function major(dev) {
    return dev >> 8;
  },
  minor: function minor(dev) {
    return dev & 0xff;
  },
  makedev: function makedev(ma, mi) {
    return ma << 8 | mi;
  },
  registerDevice: function registerDevice(dev, ops) {
    FS.devices[dev] = {
      stream_ops: ops
    };
  },
  getDevice: function getDevice(dev) {
    return FS.devices[dev];
  },
  getMounts: function getMounts(mount) {
    var mounts = [];
    var check = [mount];
    while (check.length) {
      var m = check.pop();
      mounts.push(m);
      check.push.apply(check, _toConsumableArray(m.mounts));
    }
    return mounts;
  },
  syncfs: function syncfs(populate, callback) {
    if (typeof populate == 'function') {
      callback = populate;
      populate = false;
    }
    FS.syncFSRequests++;
    if (FS.syncFSRequests > 1) {
      err("warning: ".concat(FS.syncFSRequests, " FS.syncfs operations in flight at once, probably just doing extra work"));
    }
    var mounts = FS.getMounts(FS.root.mount);
    var completed = 0;
    function doCallback(errCode) {
      FS.syncFSRequests--;
      return callback(errCode);
    }
    function done(errCode) {
      if (errCode) {
        if (!done.errored) {
          done.errored = true;
          return doCallback(errCode);
        }
        return;
      }
      if (++completed >= mounts.length) {
        doCallback(null);
      }
    }
    ;

    // sync all mounts
    mounts.forEach(function (mount) {
      if (!mount.type.syncfs) {
        return done(null);
      }
      mount.type.syncfs(mount, populate, done);
    });
  },
  mount: function mount(type, opts, mountpoint) {
    var root = mountpoint === '/';
    var pseudo = !mountpoint;
    var node;
    if (root && FS.root) {
      throw new FS.ErrnoError(10);
    } else if (!root && !pseudo) {
      var lookup = FS.lookupPath(mountpoint, {
        follow_mount: false
      });
      mountpoint = lookup.path; // use the absolute path
      node = lookup.node;
      if (FS.isMountpoint(node)) {
        throw new FS.ErrnoError(10);
      }
      if (!FS.isDir(node.mode)) {
        throw new FS.ErrnoError(54);
      }
    }
    var mount = {
      type: type,
      opts: opts,
      mountpoint: mountpoint,
      mounts: []
    };

    // create a root node for the fs
    var mountRoot = type.mount(mount);
    mountRoot.mount = mount;
    mount.root = mountRoot;
    if (root) {
      FS.root = mountRoot;
    } else if (node) {
      // set as a mountpoint
      node.mounted = mount;

      // add the new mount to the current mount's children
      if (node.mount) {
        node.mount.mounts.push(mount);
      }
    }
    return mountRoot;
  },
  unmount: function unmount(mountpoint) {
    var lookup = FS.lookupPath(mountpoint, {
      follow_mount: false
    });
    if (!FS.isMountpoint(lookup.node)) {
      throw new FS.ErrnoError(28);
    }

    // destroy the nodes for this mount, and all its child mounts
    var node = lookup.node;
    var mount = node.mounted;
    var mounts = FS.getMounts(mount);
    Object.keys(FS.nameTable).forEach(function (hash) {
      var current = FS.nameTable[hash];
      while (current) {
        var next = current.name_next;
        if (mounts.includes(current.mount)) {
          FS.destroyNode(current);
        }
        current = next;
      }
    });

    // no longer a mountpoint
    node.mounted = null;

    // remove this mount from the child mounts
    var idx = node.mount.mounts.indexOf(mount);
    node.mount.mounts.splice(idx, 1);
  },
  lookup: function lookup(parent, name) {
    return parent.node_ops.lookup(parent, name);
  },
  mknod: function mknod(path, mode, dev) {
    var lookup = FS.lookupPath(path, {
      parent: true
    });
    var parent = lookup.node;
    var name = PATH.basename(path);
    if (!name) {
      throw new FS.ErrnoError(28);
    }
    if (name === '.' || name === '..') {
      throw new FS.ErrnoError(20);
    }
    var errCode = FS.mayCreate(parent, name);
    if (errCode) {
      throw new FS.ErrnoError(errCode);
    }
    if (!parent.node_ops.mknod) {
      throw new FS.ErrnoError(63);
    }
    return parent.node_ops.mknod(parent, name, mode, dev);
  },
  statfs: function statfs(path) {
    return FS.statfsNode(FS.lookupPath(path, {
      follow: true
    }).node);
  },
  statfsStream: function statfsStream(stream) {
    // We keep a separate statfsStream function because noderawfs overrides
    // it. In noderawfs, stream.node is sometimes null. Instead, we need to
    // look at stream.path.
    return FS.statfsNode(stream.node);
  },
  statfsNode: function statfsNode(node) {
    // NOTE: None of the defaults here are true. We're just returning safe and
    //       sane values. Currently nodefs and rawfs replace these defaults,
    //       other file systems leave them alone.
    var rtn = {
      bsize: 4096,
      frsize: 4096,
      blocks: 1e6,
      bfree: 5e5,
      bavail: 5e5,
      files: FS.nextInode,
      ffree: FS.nextInode - 1,
      fsid: 42,
      flags: 2,
      namelen: 255
    };
    if (node.node_ops.statfs) {
      Object.assign(rtn, node.node_ops.statfs(node.mount.opts.root));
    }
    return rtn;
  },
  create: function create(path) {
    var mode = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : 438;
    mode &= 4095;
    mode |= 32768;
    return FS.mknod(path, mode, 0);
  },
  mkdir: function mkdir(path) {
    var mode = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : 511;
    mode &= 511 | 512;
    mode |= 16384;
    return FS.mknod(path, mode, 0);
  },
  mkdirTree: function mkdirTree(path, mode) {
    var dirs = path.split('/');
    var d = '';
    var _iterator3 = _createForOfIteratorHelper(dirs),
      _step3;
    try {
      for (_iterator3.s(); !(_step3 = _iterator3.n()).done;) {
        var dir = _step3.value;
        if (!dir) continue;
        if (d || PATH.isAbs(path)) d += '/';
        d += dir;
        try {
          FS.mkdir(d, mode);
        } catch (e) {
          if (e.errno != 20) throw e;
        }
      }
    } catch (err) {
      _iterator3.e(err);
    } finally {
      _iterator3.f();
    }
  },
  mkdev: function mkdev(path, mode, dev) {
    if (typeof dev == 'undefined') {
      dev = mode;
      mode = 438;
    }
    mode |= 8192;
    return FS.mknod(path, mode, dev);
  },
  symlink: function symlink(oldpath, newpath) {
    if (!PATH_FS.resolve(oldpath)) {
      throw new FS.ErrnoError(44);
    }
    var lookup = FS.lookupPath(newpath, {
      parent: true
    });
    var parent = lookup.node;
    if (!parent) {
      throw new FS.ErrnoError(44);
    }
    var newname = PATH.basename(newpath);
    var errCode = FS.mayCreate(parent, newname);
    if (errCode) {
      throw new FS.ErrnoError(errCode);
    }
    if (!parent.node_ops.symlink) {
      throw new FS.ErrnoError(63);
    }
    return parent.node_ops.symlink(parent, newname, oldpath);
  },
  rename: function rename(old_path, new_path) {
    var old_dirname = PATH.dirname(old_path);
    var new_dirname = PATH.dirname(new_path);
    var old_name = PATH.basename(old_path);
    var new_name = PATH.basename(new_path);
    // parents must exist
    var lookup, old_dir, new_dir;

    // let the errors from non existent directories percolate up
    lookup = FS.lookupPath(old_path, {
      parent: true
    });
    old_dir = lookup.node;
    lookup = FS.lookupPath(new_path, {
      parent: true
    });
    new_dir = lookup.node;
    if (!old_dir || !new_dir) throw new FS.ErrnoError(44);
    // need to be part of the same mount
    if (old_dir.mount !== new_dir.mount) {
      throw new FS.ErrnoError(75);
    }
    // source must exist
    var old_node = FS.lookupNode(old_dir, old_name);
    // old path should not be an ancestor of the new path
    var relative = PATH_FS.relative(old_path, new_dirname);
    if (relative.charAt(0) !== '.') {
      throw new FS.ErrnoError(28);
    }
    // new path should not be an ancestor of the old path
    relative = PATH_FS.relative(new_path, old_dirname);
    if (relative.charAt(0) !== '.') {
      throw new FS.ErrnoError(55);
    }
    // see if the new path already exists
    var new_node;
    try {
      new_node = FS.lookupNode(new_dir, new_name);
    } catch (e) {
      // not fatal
    }
    // early out if nothing needs to change
    if (old_node === new_node) {
      return;
    }
    // we'll need to delete the old entry
    var isdir = FS.isDir(old_node.mode);
    var errCode = FS.mayDelete(old_dir, old_name, isdir);
    if (errCode) {
      throw new FS.ErrnoError(errCode);
    }
    // need delete permissions if we'll be overwriting.
    // need create permissions if new doesn't already exist.
    errCode = new_node ? FS.mayDelete(new_dir, new_name, isdir) : FS.mayCreate(new_dir, new_name);
    if (errCode) {
      throw new FS.ErrnoError(errCode);
    }
    if (!old_dir.node_ops.rename) {
      throw new FS.ErrnoError(63);
    }
    if (FS.isMountpoint(old_node) || new_node && FS.isMountpoint(new_node)) {
      throw new FS.ErrnoError(10);
    }
    // if we are going to change the parent, check write permissions
    if (new_dir !== old_dir) {
      errCode = FS.nodePermissions(old_dir, 'w');
      if (errCode) {
        throw new FS.ErrnoError(errCode);
      }
    }
    // remove the node from the lookup hash
    FS.hashRemoveNode(old_node);
    // do the underlying fs rename
    try {
      old_dir.node_ops.rename(old_node, new_dir, new_name);
      // update old node (we do this here to avoid each backend
      // needing to)
      old_node.parent = new_dir;
    } catch (e) {
      throw e;
    } finally {
      // add the node back to the hash (in case node_ops.rename
      // changed its name)
      FS.hashAddNode(old_node);
    }
  },
  rmdir: function rmdir(path) {
    var lookup = FS.lookupPath(path, {
      parent: true
    });
    var parent = lookup.node;
    var name = PATH.basename(path);
    var node = FS.lookupNode(parent, name);
    var errCode = FS.mayDelete(parent, name, true);
    if (errCode) {
      throw new FS.ErrnoError(errCode);
    }
    if (!parent.node_ops.rmdir) {
      throw new FS.ErrnoError(63);
    }
    if (FS.isMountpoint(node)) {
      throw new FS.ErrnoError(10);
    }
    parent.node_ops.rmdir(parent, name);
    FS.destroyNode(node);
  },
  readdir: function readdir(path) {
    var lookup = FS.lookupPath(path, {
      follow: true
    });
    var node = lookup.node;
    var readdir = FS.checkOpExists(node.node_ops.readdir, 54);
    return readdir(node);
  },
  unlink: function unlink(path) {
    var lookup = FS.lookupPath(path, {
      parent: true
    });
    var parent = lookup.node;
    if (!parent) {
      throw new FS.ErrnoError(44);
    }
    var name = PATH.basename(path);
    var node = FS.lookupNode(parent, name);
    var errCode = FS.mayDelete(parent, name, false);
    if (errCode) {
      // According to POSIX, we should map EISDIR to EPERM, but
      // we instead do what Linux does (and we must, as we use
      // the musl linux libc).
      throw new FS.ErrnoError(errCode);
    }
    if (!parent.node_ops.unlink) {
      throw new FS.ErrnoError(63);
    }
    if (FS.isMountpoint(node)) {
      throw new FS.ErrnoError(10);
    }
    parent.node_ops.unlink(parent, name);
    FS.destroyNode(node);
  },
  readlink: function readlink(path) {
    var lookup = FS.lookupPath(path);
    var link = lookup.node;
    if (!link) {
      throw new FS.ErrnoError(44);
    }
    if (!link.node_ops.readlink) {
      throw new FS.ErrnoError(28);
    }
    return link.node_ops.readlink(link);
  },
  stat: function stat(path, dontFollow) {
    var lookup = FS.lookupPath(path, {
      follow: !dontFollow
    });
    var node = lookup.node;
    var getattr = FS.checkOpExists(node.node_ops.getattr, 63);
    return getattr(node);
  },
  fstat: function fstat(fd) {
    var stream = FS.getStreamChecked(fd);
    var node = stream.node;
    var getattr = stream.stream_ops.getattr;
    var arg = getattr ? stream : node;
    getattr !== null && getattr !== void 0 ? getattr : getattr = node.node_ops.getattr;
    FS.checkOpExists(getattr, 63);
    return getattr(arg);
  },
  lstat: function lstat(path) {
    return FS.stat(path, true);
  },
  doChmod: function doChmod(stream, node, mode, dontFollow) {
    FS.doSetAttr(stream, node, {
      mode: mode & 4095 | node.mode & ~4095,
      ctime: Date.now(),
      dontFollow: dontFollow
    });
  },
  chmod: function chmod(path, mode, dontFollow) {
    var node;
    if (typeof path == 'string') {
      var lookup = FS.lookupPath(path, {
        follow: !dontFollow
      });
      node = lookup.node;
    } else {
      node = path;
    }
    FS.doChmod(null, node, mode, dontFollow);
  },
  lchmod: function lchmod(path, mode) {
    FS.chmod(path, mode, true);
  },
  fchmod: function fchmod(fd, mode) {
    var stream = FS.getStreamChecked(fd);
    FS.doChmod(stream, stream.node, mode, false);
  },
  doChown: function doChown(stream, node, dontFollow) {
    FS.doSetAttr(stream, node, {
      timestamp: Date.now(),
      dontFollow: dontFollow
      // we ignore the uid / gid for now
    });
  },
  chown: function chown(path, uid, gid, dontFollow) {
    var node;
    if (typeof path == 'string') {
      var lookup = FS.lookupPath(path, {
        follow: !dontFollow
      });
      node = lookup.node;
    } else {
      node = path;
    }
    FS.doChown(null, node, dontFollow);
  },
  lchown: function lchown(path, uid, gid) {
    FS.chown(path, uid, gid, true);
  },
  fchown: function fchown(fd, uid, gid) {
    var stream = FS.getStreamChecked(fd);
    FS.doChown(stream, stream.node, false);
  },
  doTruncate: function doTruncate(stream, node, len) {
    if (FS.isDir(node.mode)) {
      throw new FS.ErrnoError(31);
    }
    if (!FS.isFile(node.mode)) {
      throw new FS.ErrnoError(28);
    }
    var errCode = FS.nodePermissions(node, 'w');
    if (errCode) {
      throw new FS.ErrnoError(errCode);
    }
    FS.doSetAttr(stream, node, {
      size: len,
      timestamp: Date.now()
    });
  },
  truncate: function truncate(path, len) {
    if (len < 0) {
      throw new FS.ErrnoError(28);
    }
    var node;
    if (typeof path == 'string') {
      var lookup = FS.lookupPath(path, {
        follow: true
      });
      node = lookup.node;
    } else {
      node = path;
    }
    FS.doTruncate(null, node, len);
  },
  ftruncate: function ftruncate(fd, len) {
    var stream = FS.getStreamChecked(fd);
    if (len < 0 || (stream.flags & 2097155) === 0) {
      throw new FS.ErrnoError(28);
    }
    FS.doTruncate(stream, stream.node, len);
  },
  utime: function utime(path, atime, mtime) {
    var lookup = FS.lookupPath(path, {
      follow: true
    });
    var node = lookup.node;
    var setattr = FS.checkOpExists(node.node_ops.setattr, 63);
    setattr(node, {
      atime: atime,
      mtime: mtime
    });
  },
  open: function open(path, flags) {
    var mode = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : 438;
    if (path === "") {
      throw new FS.ErrnoError(44);
    }
    flags = typeof flags == 'string' ? FS_modeStringToFlags(flags) : flags;
    if (flags & 64) {
      mode = mode & 4095 | 32768;
    } else {
      mode = 0;
    }
    var node;
    var isDirPath;
    if (_typeof(path) == 'object') {
      node = path;
    } else {
      isDirPath = path.endsWith("/");
      // noent_okay makes it so that if the final component of the path
      // doesn't exist, lookupPath returns `node: undefined`. `path` will be
      // updated to point to the target of all symlinks.
      var lookup = FS.lookupPath(path, {
        follow: !(flags & 131072),
        noent_okay: true
      });
      node = lookup.node;
      path = lookup.path;
    }
    // perhaps we need to create the node
    var created = false;
    if (flags & 64) {
      if (node) {
        // if O_CREAT and O_EXCL are set, error out if the node already exists
        if (flags & 128) {
          throw new FS.ErrnoError(20);
        }
      } else if (isDirPath) {
        throw new FS.ErrnoError(31);
      } else {
        // node doesn't exist, try to create it
        // Ignore the permission bits here to ensure we can `open` this new
        // file below. We use chmod below the apply the permissions once the
        // file is open.
        node = FS.mknod(path, mode | 511, 0);
        created = true;
      }
    }
    if (!node) {
      throw new FS.ErrnoError(44);
    }
    // can't truncate a device
    if (FS.isChrdev(node.mode)) {
      flags &= ~512;
    }
    // if asked only for a directory, then this must be one
    if (flags & 65536 && !FS.isDir(node.mode)) {
      throw new FS.ErrnoError(54);
    }
    // check permissions, if this is not a file we just created now (it is ok to
    // create and write to a file with read-only permissions; it is read-only
    // for later use)
    if (!created) {
      var errCode = FS.mayOpen(node, flags);
      if (errCode) {
        throw new FS.ErrnoError(errCode);
      }
    }
    // do truncation if necessary
    if (flags & 512 && !created) {
      FS.truncate(node, 0);
    }
    // we've already handled these, don't pass down to the underlying vfs
    flags &= ~(128 | 512 | 131072);

    // register the stream with the filesystem
    var stream = FS.createStream({
      node: node,
      path: FS.getPath(node),
      // we want the absolute path to the node
      flags: flags,
      seekable: true,
      position: 0,
      stream_ops: node.stream_ops,
      // used by the file family libc calls (fopen, fwrite, ferror, etc.)
      ungotten: [],
      error: false
    });
    // call the new stream's open function
    if (stream.stream_ops.open) {
      stream.stream_ops.open(stream);
    }
    if (created) {
      FS.chmod(node, mode & 511);
    }
    if (Module['logReadFiles'] && !(flags & 1)) {
      if (!(path in FS.readFiles)) {
        FS.readFiles[path] = 1;
      }
    }
    return stream;
  },
  close: function close(stream) {
    if (FS.isClosed(stream)) {
      throw new FS.ErrnoError(8);
    }
    if (stream.getdents) stream.getdents = null; // free readdir state
    try {
      if (stream.stream_ops.close) {
        stream.stream_ops.close(stream);
      }
    } catch (e) {
      throw e;
    } finally {
      FS.closeStream(stream.fd);
    }
    stream.fd = null;
  },
  isClosed: function isClosed(stream) {
    return stream.fd === null;
  },
  llseek: function llseek(stream, offset, whence) {
    if (FS.isClosed(stream)) {
      throw new FS.ErrnoError(8);
    }
    if (!stream.seekable || !stream.stream_ops.llseek) {
      throw new FS.ErrnoError(70);
    }
    if (whence != 0 && whence != 1 && whence != 2) {
      throw new FS.ErrnoError(28);
    }
    stream.position = stream.stream_ops.llseek(stream, offset, whence);
    stream.ungotten = [];
    return stream.position;
  },
  read: function read(stream, buffer, offset, length, position) {
    if (length < 0 || position < 0) {
      throw new FS.ErrnoError(28);
    }
    if (FS.isClosed(stream)) {
      throw new FS.ErrnoError(8);
    }
    if ((stream.flags & 2097155) === 1) {
      throw new FS.ErrnoError(8);
    }
    if (FS.isDir(stream.node.mode)) {
      throw new FS.ErrnoError(31);
    }
    if (!stream.stream_ops.read) {
      throw new FS.ErrnoError(28);
    }
    var seeking = typeof position != 'undefined';
    if (!seeking) {
      position = stream.position;
    } else if (!stream.seekable) {
      throw new FS.ErrnoError(70);
    }
    var bytesRead = stream.stream_ops.read(stream, buffer, offset, length, position);
    if (!seeking) stream.position += bytesRead;
    return bytesRead;
  },
  write: function write(stream, buffer, offset, length, position, canOwn) {
    if (length < 0 || position < 0) {
      throw new FS.ErrnoError(28);
    }
    if (FS.isClosed(stream)) {
      throw new FS.ErrnoError(8);
    }
    if ((stream.flags & 2097155) === 0) {
      throw new FS.ErrnoError(8);
    }
    if (FS.isDir(stream.node.mode)) {
      throw new FS.ErrnoError(31);
    }
    if (!stream.stream_ops.write) {
      throw new FS.ErrnoError(28);
    }
    if (stream.seekable && stream.flags & 1024) {
      // seek to the end before writing in append mode
      FS.llseek(stream, 0, 2);
    }
    var seeking = typeof position != 'undefined';
    if (!seeking) {
      position = stream.position;
    } else if (!stream.seekable) {
      throw new FS.ErrnoError(70);
    }
    var bytesWritten = stream.stream_ops.write(stream, buffer, offset, length, position, canOwn);
    if (!seeking) stream.position += bytesWritten;
    return bytesWritten;
  },
  mmap: function mmap(stream, length, position, prot, flags) {
    // User requests writing to file (prot & PROT_WRITE != 0).
    // Checking if we have permissions to write to the file unless
    // MAP_PRIVATE flag is set. According to POSIX spec it is possible
    // to write to file opened in read-only mode with MAP_PRIVATE flag,
    // as all modifications will be visible only in the memory of
    // the current process.
    if ((prot & 2) !== 0 && (flags & 2) === 0 && (stream.flags & 2097155) !== 2) {
      throw new FS.ErrnoError(2);
    }
    if ((stream.flags & 2097155) === 1) {
      throw new FS.ErrnoError(2);
    }
    if (!stream.stream_ops.mmap) {
      throw new FS.ErrnoError(43);
    }
    if (!length) {
      throw new FS.ErrnoError(28);
    }
    return stream.stream_ops.mmap(stream, length, position, prot, flags);
  },
  msync: function msync(stream, buffer, offset, length, mmapFlags) {
    if (!stream.stream_ops.msync) {
      return 0;
    }
    return stream.stream_ops.msync(stream, buffer, offset, length, mmapFlags);
  },
  ioctl: function ioctl(stream, cmd, arg) {
    if (!stream.stream_ops.ioctl) {
      throw new FS.ErrnoError(59);
    }
    return stream.stream_ops.ioctl(stream, cmd, arg);
  },
  readFile: function readFile(path) {
    var opts = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : {};
    opts.flags = opts.flags || 0;
    opts.encoding = opts.encoding || 'binary';
    if (opts.encoding !== 'utf8' && opts.encoding !== 'binary') {
      throw new Error("Invalid encoding type \"".concat(opts.encoding, "\""));
    }
    var ret;
    var stream = FS.open(path, opts.flags);
    var stat = FS.stat(path);
    var length = stat.size;
    var buf = new Uint8Array(length);
    FS.read(stream, buf, 0, length, 0);
    if (opts.encoding === 'utf8') {
      ret = UTF8ArrayToString(buf);
    } else if (opts.encoding === 'binary') {
      ret = buf;
    }
    FS.close(stream);
    return ret;
  },
  writeFile: function writeFile(path, data) {
    var opts = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : {};
    opts.flags = opts.flags || 577;
    var stream = FS.open(path, opts.flags, opts.mode);
    if (typeof data == 'string') {
      var buf = new Uint8Array(lengthBytesUTF8(data) + 1);
      var actualNumBytes = stringToUTF8Array(data, buf, 0, buf.length);
      FS.write(stream, buf, 0, actualNumBytes, undefined, opts.canOwn);
    } else if (ArrayBuffer.isView(data)) {
      FS.write(stream, data, 0, data.byteLength, undefined, opts.canOwn);
    } else {
      throw new Error('Unsupported data type');
    }
    FS.close(stream);
  },
  cwd: function cwd() {
    return FS.currentPath;
  },
  chdir: function chdir(path) {
    var lookup = FS.lookupPath(path, {
      follow: true
    });
    if (lookup.node === null) {
      throw new FS.ErrnoError(44);
    }
    if (!FS.isDir(lookup.node.mode)) {
      throw new FS.ErrnoError(54);
    }
    var errCode = FS.nodePermissions(lookup.node, 'x');
    if (errCode) {
      throw new FS.ErrnoError(errCode);
    }
    FS.currentPath = lookup.path;
  },
  createDefaultDirectories: function createDefaultDirectories() {
    FS.mkdir('/tmp');
    FS.mkdir('/home');
    FS.mkdir('/home/web_user');
  },
  createDefaultDevices: function createDefaultDevices() {
    // create /dev
    FS.mkdir('/dev');
    // setup /dev/null
    FS.registerDevice(FS.makedev(1, 3), {
      read: function read() {
        return 0;
      },
      write: function write(stream, buffer, offset, length, pos) {
        return length;
      },
      llseek: function llseek() {
        return 0;
      }
    });
    FS.mkdev('/dev/null', FS.makedev(1, 3));
    // setup /dev/tty and /dev/tty1
    // stderr needs to print output using err() rather than out()
    // so we register a second tty just for it.
    TTY.register(FS.makedev(5, 0), TTY.default_tty_ops);
    TTY.register(FS.makedev(6, 0), TTY.default_tty1_ops);
    FS.mkdev('/dev/tty', FS.makedev(5, 0));
    FS.mkdev('/dev/tty1', FS.makedev(6, 0));
    // setup /dev/[u]random
    // use a buffer to avoid overhead of individual crypto calls per byte
    var randomBuffer = new Uint8Array(1024),
      randomLeft = 0;
    var randomByte = function randomByte() {
      if (randomLeft === 0) {
        _randomFill(randomBuffer);
        randomLeft = randomBuffer.byteLength;
      }
      return randomBuffer[--randomLeft];
    };
    FS.createDevice('/dev', 'random', randomByte);
    FS.createDevice('/dev', 'urandom', randomByte);
    // we're not going to emulate the actual shm device,
    // just create the tmp dirs that reside in it commonly
    FS.mkdir('/dev/shm');
    FS.mkdir('/dev/shm/tmp');
  },
  createSpecialDirectories: function createSpecialDirectories() {
    // create /proc/self/fd which allows /proc/self/fd/6 => readlink gives the
    // name of the stream for fd 6 (see test_unistd_ttyname)
    FS.mkdir('/proc');
    var proc_self = FS.mkdir('/proc/self');
    FS.mkdir('/proc/self/fd');
    FS.mount({
      mount: function mount() {
        var node = FS.createNode(proc_self, 'fd', 16895, 73);
        node.stream_ops = {
          llseek: MEMFS.stream_ops.llseek
        };
        node.node_ops = {
          lookup: function lookup(parent, name) {
            var fd = +name;
            var stream = FS.getStreamChecked(fd);
            var ret = {
              parent: null,
              mount: {
                mountpoint: 'fake'
              },
              node_ops: {
                readlink: function readlink() {
                  return stream.path;
                }
              },
              id: fd + 1
            };
            ret.parent = ret; // make it look like a simple root node
            return ret;
          },
          readdir: function readdir() {
            return Array.from(FS.streams.entries()).filter(function (_ref4) {
              var _ref5 = _slicedToArray(_ref4, 2),
                k = _ref5[0],
                v = _ref5[1];
              return v;
            }).map(function (_ref6) {
              var _ref7 = _slicedToArray(_ref6, 2),
                k = _ref7[0],
                v = _ref7[1];
              return k.toString();
            });
          }
        };
        return node;
      }
    }, {}, '/proc/self/fd');
  },
  createStandardStreams: function createStandardStreams(input, output, error) {
    // TODO deprecate the old functionality of a single
    // input / output callback and that utilizes FS.createDevice
    // and instead require a unique set of stream ops

    // by default, we symlink the standard streams to the
    // default tty devices. however, if the standard streams
    // have been overwritten we create a unique device for
    // them instead.
    if (input) {
      FS.createDevice('/dev', 'stdin', input);
    } else {
      FS.symlink('/dev/tty', '/dev/stdin');
    }
    if (output) {
      FS.createDevice('/dev', 'stdout', null, output);
    } else {
      FS.symlink('/dev/tty', '/dev/stdout');
    }
    if (error) {
      FS.createDevice('/dev', 'stderr', null, error);
    } else {
      FS.symlink('/dev/tty1', '/dev/stderr');
    }

    // open default streams for the stdin, stdout and stderr devices
    var stdin = FS.open('/dev/stdin', 0);
    var stdout = FS.open('/dev/stdout', 1);
    var stderr = FS.open('/dev/stderr', 1);
  },
  staticInit: function staticInit() {
    FS.nameTable = new Array(4096);
    FS.mount(MEMFS, {}, '/');
    FS.createDefaultDirectories();
    FS.createDefaultDevices();
    FS.createSpecialDirectories();
    FS.filesystems = {
      'MEMFS': MEMFS
    };
  },
  init: function init(input, output, error) {
    FS.initialized = true;

    // Allow Module.stdin etc. to provide defaults, if none explicitly passed to us here
    input !== null && input !== void 0 ? input : input = Module['stdin'];
    output !== null && output !== void 0 ? output : output = Module['stdout'];
    error !== null && error !== void 0 ? error : error = Module['stderr'];
    FS.createStandardStreams(input, output, error);
  },
  quit: function quit() {
    FS.initialized = false;
    // force-flush all streams, so we get musl std streams printed out
    // close all of our streams
    var _iterator4 = _createForOfIteratorHelper(FS.streams),
      _step4;
    try {
      for (_iterator4.s(); !(_step4 = _iterator4.n()).done;) {
        var stream = _step4.value;
        if (stream) {
          FS.close(stream);
        }
      }
    } catch (err) {
      _iterator4.e(err);
    } finally {
      _iterator4.f();
    }
  },
  findObject: function findObject(path, dontResolveLastLink) {
    var ret = FS.analyzePath(path, dontResolveLastLink);
    if (!ret.exists) {
      return null;
    }
    return ret.object;
  },
  analyzePath: function analyzePath(path, dontResolveLastLink) {
    // operate from within the context of the symlink's target
    try {
      var lookup = FS.lookupPath(path, {
        follow: !dontResolveLastLink
      });
      path = lookup.path;
    } catch (e) {}
    var ret = {
      isRoot: false,
      exists: false,
      error: 0,
      name: null,
      path: null,
      object: null,
      parentExists: false,
      parentPath: null,
      parentObject: null
    };
    try {
      var lookup = FS.lookupPath(path, {
        parent: true
      });
      ret.parentExists = true;
      ret.parentPath = lookup.path;
      ret.parentObject = lookup.node;
      ret.name = PATH.basename(path);
      lookup = FS.lookupPath(path, {
        follow: !dontResolveLastLink
      });
      ret.exists = true;
      ret.path = lookup.path;
      ret.object = lookup.node;
      ret.name = lookup.node.name;
      ret.isRoot = lookup.path === '/';
    } catch (e) {
      ret.error = e.errno;
    }
    ;
    return ret;
  },
  createPath: function createPath(parent, path, canRead, canWrite) {
    parent = typeof parent == 'string' ? parent : FS.getPath(parent);
    var parts = path.split('/').reverse();
    while (parts.length) {
      var part = parts.pop();
      if (!part) continue;
      var current = PATH.join2(parent, part);
      try {
        FS.mkdir(current);
      } catch (e) {
        if (e.errno != 20) throw e;
      }
      parent = current;
    }
    return current;
  },
  createFile: function createFile(parent, name, properties, canRead, canWrite) {
    var path = PATH.join2(typeof parent == 'string' ? parent : FS.getPath(parent), name);
    var mode = FS_getMode(canRead, canWrite);
    return FS.create(path, mode);
  },
  createDataFile: function createDataFile(parent, name, data, canRead, canWrite, canOwn) {
    var path = name;
    if (parent) {
      parent = typeof parent == 'string' ? parent : FS.getPath(parent);
      path = name ? PATH.join2(parent, name) : parent;
    }
    var mode = FS_getMode(canRead, canWrite);
    var node = FS.create(path, mode);
    if (data) {
      if (typeof data == 'string') {
        var arr = new Array(data.length);
        for (var i = 0, len = data.length; i < len; ++i) arr[i] = data.charCodeAt(i);
        data = arr;
      }
      // make sure we can write to the file
      FS.chmod(node, mode | 146);
      var stream = FS.open(node, 577);
      FS.write(stream, data, 0, data.length, 0, canOwn);
      FS.close(stream);
      FS.chmod(node, mode);
    }
  },
  createDevice: function createDevice(parent, name, input, output) {
    var _FS$createDevice, _FS$createDevice$majo;
    var path = PATH.join2(typeof parent == 'string' ? parent : FS.getPath(parent), name);
    var mode = FS_getMode(!!input, !!output);
    (_FS$createDevice$majo = (_FS$createDevice = FS.createDevice).major) !== null && _FS$createDevice$majo !== void 0 ? _FS$createDevice$majo : _FS$createDevice.major = 64;
    var dev = FS.makedev(FS.createDevice.major++, 0);
    // Create a fake device that a set of stream ops to emulate
    // the old behavior.
    FS.registerDevice(dev, {
      open: function open(stream) {
        stream.seekable = false;
      },
      close: function close(stream) {
        var _output$buffer;
        // flush any pending line data
        if (output !== null && output !== void 0 && (_output$buffer = output.buffer) !== null && _output$buffer !== void 0 && _output$buffer.length) {
          output(10);
        }
      },
      read: function read(stream, buffer, offset, length, pos /* ignored */) {
        var bytesRead = 0;
        for (var i = 0; i < length; i++) {
          var result;
          try {
            result = input();
          } catch (e) {
            throw new FS.ErrnoError(29);
          }
          if (result === undefined && bytesRead === 0) {
            throw new FS.ErrnoError(6);
          }
          if (result === null || result === undefined) break;
          bytesRead++;
          buffer[offset + i] = result;
        }
        if (bytesRead) {
          stream.node.atime = Date.now();
        }
        return bytesRead;
      },
      write: function write(stream, buffer, offset, length, pos) {
        for (var i = 0; i < length; i++) {
          try {
            output(buffer[offset + i]);
          } catch (e) {
            throw new FS.ErrnoError(29);
          }
        }
        if (length) {
          stream.node.mtime = stream.node.ctime = Date.now();
        }
        return i;
      }
    });
    return FS.mkdev(path, mode, dev);
  },
  forceLoadFile: function forceLoadFile(obj) {
    if (obj.isDevice || obj.isFolder || obj.link || obj.contents) return true;
    if (typeof XMLHttpRequest != 'undefined') {
      throw new Error("Lazy loading should have been performed (contents set) in createLazyFile, but it was not. Lazy loading only works in web workers. Use --embed-file or --preload-file in emcc on the main thread.");
    } else {
      // Command-line.
      try {
        obj.contents = readBinary(obj.url);
        obj.usedBytes = obj.contents.length;
      } catch (e) {
        throw new FS.ErrnoError(29);
      }
    }
  },
  createLazyFile: function createLazyFile(parent, name, url, canRead, canWrite) {
    // Lazy chunked Uint8Array (implements get and length from Uint8Array).
    // Actual getting is abstracted away for eventual reuse.
    var LazyUint8Array = /*#__PURE__*/function () {
      "use strict";

      function LazyUint8Array() {
        _classCallCheck(this, LazyUint8Array);
        _defineProperty(this, "lengthKnown", false);
        _defineProperty(this, "chunks", []);
      }
      return _createClass(LazyUint8Array, [{
        key: "get",
        value:
        // Loaded chunks. Index is the chunk number
        function get(idx) {
          if (idx > this.length - 1 || idx < 0) {
            return undefined;
          }
          var chunkOffset = idx % this.chunkSize;
          var chunkNum = idx / this.chunkSize | 0;
          return this.getter(chunkNum)[chunkOffset];
        }
      }, {
        key: "setDataGetter",
        value: function setDataGetter(getter) {
          this.getter = getter;
        }
      }, {
        key: "cacheLength",
        value: function cacheLength() {
          // Find length
          var xhr = new XMLHttpRequest();
          xhr.open('HEAD', url, false);
          xhr.send(null);
          if (!(xhr.status >= 200 && xhr.status < 300 || xhr.status === 304)) throw new Error("Couldn't load " + url + ". Status: " + xhr.status);
          var datalength = Number(xhr.getResponseHeader("Content-length"));
          var header;
          var hasByteServing = (header = xhr.getResponseHeader("Accept-Ranges")) && header === "bytes";
          var usesGzip = (header = xhr.getResponseHeader("Content-Encoding")) && header === "gzip";
          var chunkSize = 1024 * 1024; // Chunk size in bytes

          if (!hasByteServing) chunkSize = datalength;

          // Function to get a range from the remote URL.
          var doXHR = function doXHR(from, to) {
            if (from > to) throw new Error("invalid range (" + from + ", " + to + ") or no bytes requested!");
            if (to > datalength - 1) throw new Error("only " + datalength + " bytes available! programmer error!");

            // TODO: Use mozResponseArrayBuffer, responseStream, etc. if available.
            var xhr = new XMLHttpRequest();
            xhr.open('GET', url, false);
            if (datalength !== chunkSize) xhr.setRequestHeader("Range", "bytes=" + from + "-" + to);

            // Some hints to the browser that we want binary data.
            xhr.responseType = 'arraybuffer';
            if (xhr.overrideMimeType) {
              xhr.overrideMimeType('text/plain; charset=x-user-defined');
            }
            xhr.send(null);
            if (!(xhr.status >= 200 && xhr.status < 300 || xhr.status === 304)) throw new Error("Couldn't load " + url + ". Status: " + xhr.status);
            if (xhr.response !== undefined) {
              return new Uint8Array(/** @type{Array<number>} */xhr.response || []);
            }
            return intArrayFromString(xhr.responseText || '', true);
          };
          var lazyArray = this;
          lazyArray.setDataGetter(function (chunkNum) {
            var start = chunkNum * chunkSize;
            var end = (chunkNum + 1) * chunkSize - 1; // including this byte
            end = Math.min(end, datalength - 1); // if datalength-1 is selected, this is the last block
            if (typeof lazyArray.chunks[chunkNum] == 'undefined') {
              lazyArray.chunks[chunkNum] = doXHR(start, end);
            }
            if (typeof lazyArray.chunks[chunkNum] == 'undefined') throw new Error('doXHR failed!');
            return lazyArray.chunks[chunkNum];
          });
          if (usesGzip || !datalength) {
            // if the server uses gzip or doesn't supply the length, we have to download the whole file to get the (uncompressed) length
            chunkSize = datalength = 1; // this will force getter(0)/doXHR do download the whole file
            datalength = this.getter(0).length;
            chunkSize = datalength;
            out("LazyFiles on gzip forces download of the whole file when length is accessed");
          }
          this._length = datalength;
          this._chunkSize = chunkSize;
          this.lengthKnown = true;
        }
      }, {
        key: "length",
        get: function get() {
          if (!this.lengthKnown) {
            this.cacheLength();
          }
          return this._length;
        }
      }, {
        key: "chunkSize",
        get: function get() {
          if (!this.lengthKnown) {
            this.cacheLength();
          }
          return this._chunkSize;
        }
      }]);
    }();
    if (typeof XMLHttpRequest != 'undefined') {
      if (!ENVIRONMENT_IS_WORKER) throw 'Cannot do synchronous binary XHRs outside webworkers in modern browsers. Use --embed-file or --preload-file in emcc';
      var lazyArray = new LazyUint8Array();
      var properties = {
        isDevice: false,
        contents: lazyArray
      };
    } else {
      var properties = {
        isDevice: false,
        url: url
      };
    }
    var node = FS.createFile(parent, name, properties, canRead, canWrite);
    // This is a total hack, but I want to get this lazy file code out of the
    // core of MEMFS. If we want to keep this lazy file concept I feel it should
    // be its own thin LAZYFS proxying calls to MEMFS.
    if (properties.contents) {
      node.contents = properties.contents;
    } else if (properties.url) {
      node.contents = null;
      node.url = properties.url;
    }
    // Add a function that defers querying the file size until it is asked the first time.
    Object.defineProperties(node, {
      usedBytes: {
        get: function get() {
          return this.contents.length;
        }
      }
    });
    // override each stream op with one that tries to force load the lazy file first
    var stream_ops = {};
    var keys = Object.keys(node.stream_ops);
    keys.forEach(function (key) {
      var fn = node.stream_ops[key];
      stream_ops[key] = function () {
        FS.forceLoadFile(node);
        return fn.apply(void 0, arguments);
      };
    });
    function writeChunks(stream, buffer, offset, length, position) {
      var contents = stream.node.contents;
      if (position >= contents.length) return 0;
      var size = Math.min(contents.length - position, length);
      if (contents.slice) {
        // normal array
        for (var i = 0; i < size; i++) {
          buffer[offset + i] = contents[position + i];
        }
      } else {
        for (var i = 0; i < size; i++) {
          // LazyUint8Array from sync binary XHR
          buffer[offset + i] = contents.get(position + i);
        }
      }
      return size;
    }
    // use a custom read function
    stream_ops.read = function (stream, buffer, offset, length, position) {
      FS.forceLoadFile(node);
      return writeChunks(stream, buffer, offset, length, position);
    };
    // use a custom mmap function
    stream_ops.mmap = function (stream, length, position, prot, flags) {
      FS.forceLoadFile(node);
      var ptr = mmapAlloc(length);
      if (!ptr) {
        throw new FS.ErrnoError(48);
      }
      writeChunks(stream, HEAP8, ptr, length, position);
      return {
        ptr: ptr,
        allocated: true
      };
    };
    node.stream_ops = stream_ops;
    return node;
  }
};

/**
 * Given a pointer 'ptr' to a null-terminated UTF8-encoded string in the
 * emscripten HEAP, returns a copy of that string as a Javascript String object.
 *
 * @param {number} ptr
 * @param {number=} maxBytesToRead - An optional length that specifies the
 *   maximum number of bytes to read. You can omit this parameter to scan the
 *   string until the first 0 byte. If maxBytesToRead is passed, and the string
 *   at [ptr, ptr+maxBytesToReadr[ contains a null byte in the middle, then the
 *   string will cut short at that byte index (i.e. maxBytesToRead will not
 *   produce a string of exact length [ptr, ptr+maxBytesToRead[) N.B. mixing
 *   frequent uses of UTF8ToString() with and without maxBytesToRead may throw
 *   JS JIT optimizations off, so it is worth to consider consistently using one
 * @return {string}
 */
var UTF8ToString = function UTF8ToString(ptr, maxBytesToRead) {
  return ptr ? UTF8ArrayToString(HEAPU8, ptr, maxBytesToRead) : '';
};
var SYSCALLS = {
  DEFAULT_POLLMASK: 5,
  calculateAt: function calculateAt(dirfd, path, allowEmpty) {
    if (PATH.isAbs(path)) {
      return path;
    }
    // relative path
    var dir;
    if (dirfd === -100) {
      dir = FS.cwd();
    } else {
      var dirstream = SYSCALLS.getStreamFromFD(dirfd);
      dir = dirstream.path;
    }
    if (path.length == 0) {
      if (!allowEmpty) {
        throw new FS.ErrnoError(44);
        ;
      }
      return dir;
    }
    return dir + '/' + path;
  },
  writeStat: function writeStat(buf, stat) {
    HEAP32[buf >> 2] = stat.dev;
    HEAP32[buf + 4 >> 2] = stat.mode;
    HEAPU32[buf + 8 >> 2] = stat.nlink;
    HEAP32[buf + 12 >> 2] = stat.uid;
    HEAP32[buf + 16 >> 2] = stat.gid;
    HEAP32[buf + 20 >> 2] = stat.rdev;
    tempI64 = [stat.size >>> 0, (tempDouble = stat.size, +Math.abs(tempDouble) >= 1.0 ? tempDouble > 0.0 ? +Math.floor(tempDouble / 4294967296.0) >>> 0 : ~~+Math.ceil((tempDouble - +(~~tempDouble >>> 0)) / 4294967296.0) >>> 0 : 0)], HEAP32[buf + 24 >> 2] = tempI64[0], HEAP32[buf + 28 >> 2] = tempI64[1];
    HEAP32[buf + 32 >> 2] = 4096;
    HEAP32[buf + 36 >> 2] = stat.blocks;
    var atime = stat.atime.getTime();
    var mtime = stat.mtime.getTime();
    var ctime = stat.ctime.getTime();
    tempI64 = [Math.floor(atime / 1000) >>> 0, (tempDouble = Math.floor(atime / 1000), +Math.abs(tempDouble) >= 1.0 ? tempDouble > 0.0 ? +Math.floor(tempDouble / 4294967296.0) >>> 0 : ~~+Math.ceil((tempDouble - +(~~tempDouble >>> 0)) / 4294967296.0) >>> 0 : 0)], HEAP32[buf + 40 >> 2] = tempI64[0], HEAP32[buf + 44 >> 2] = tempI64[1];
    HEAPU32[buf + 48 >> 2] = atime % 1000 * 1000 * 1000;
    tempI64 = [Math.floor(mtime / 1000) >>> 0, (tempDouble = Math.floor(mtime / 1000), +Math.abs(tempDouble) >= 1.0 ? tempDouble > 0.0 ? +Math.floor(tempDouble / 4294967296.0) >>> 0 : ~~+Math.ceil((tempDouble - +(~~tempDouble >>> 0)) / 4294967296.0) >>> 0 : 0)], HEAP32[buf + 56 >> 2] = tempI64[0], HEAP32[buf + 60 >> 2] = tempI64[1];
    HEAPU32[buf + 64 >> 2] = mtime % 1000 * 1000 * 1000;
    tempI64 = [Math.floor(ctime / 1000) >>> 0, (tempDouble = Math.floor(ctime / 1000), +Math.abs(tempDouble) >= 1.0 ? tempDouble > 0.0 ? +Math.floor(tempDouble / 4294967296.0) >>> 0 : ~~+Math.ceil((tempDouble - +(~~tempDouble >>> 0)) / 4294967296.0) >>> 0 : 0)], HEAP32[buf + 72 >> 2] = tempI64[0], HEAP32[buf + 76 >> 2] = tempI64[1];
    HEAPU32[buf + 80 >> 2] = ctime % 1000 * 1000 * 1000;
    tempI64 = [stat.ino >>> 0, (tempDouble = stat.ino, +Math.abs(tempDouble) >= 1.0 ? tempDouble > 0.0 ? +Math.floor(tempDouble / 4294967296.0) >>> 0 : ~~+Math.ceil((tempDouble - +(~~tempDouble >>> 0)) / 4294967296.0) >>> 0 : 0)], HEAP32[buf + 88 >> 2] = tempI64[0], HEAP32[buf + 92 >> 2] = tempI64[1];
    return 0;
  },
  writeStatFs: function writeStatFs(buf, stats) {
    HEAP32[buf + 4 >> 2] = stats.bsize;
    HEAP32[buf + 40 >> 2] = stats.bsize;
    HEAP32[buf + 8 >> 2] = stats.blocks;
    HEAP32[buf + 12 >> 2] = stats.bfree;
    HEAP32[buf + 16 >> 2] = stats.bavail;
    HEAP32[buf + 20 >> 2] = stats.files;
    HEAP32[buf + 24 >> 2] = stats.ffree;
    HEAP32[buf + 28 >> 2] = stats.fsid;
    HEAP32[buf + 44 >> 2] = stats.flags; // ST_NOSUID
    HEAP32[buf + 36 >> 2] = stats.namelen;
  },
  doMsync: function doMsync(addr, stream, len, flags, offset) {
    if (!FS.isFile(stream.node.mode)) {
      throw new FS.ErrnoError(43);
    }
    if (flags & 2) {
      // MAP_PRIVATE calls need not to be synced back to underlying fs
      return 0;
    }
    var buffer = HEAPU8.slice(addr, addr + len);
    FS.msync(stream, buffer, offset, len, flags);
  },
  getStreamFromFD: function getStreamFromFD(fd) {
    var stream = FS.getStreamChecked(fd);
    return stream;
  },
  varargs: undefined,
  getStr: function getStr(ptr) {
    var ret = UTF8ToString(ptr);
    return ret;
  }
};
function _fd_close(fd) {
  try {
    var stream = SYSCALLS.getStreamFromFD(fd);
    FS.close(stream);
    return 0;
  } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return e.errno;
  }
}

/** @param {number=} offset */
var doReadv = function doReadv(stream, iov, iovcnt, offset) {
  var ret = 0;
  for (var i = 0; i < iovcnt; i++) {
    var ptr = HEAPU32[iov >> 2];
    var len = HEAPU32[iov + 4 >> 2];
    iov += 8;
    var curr = FS.read(stream, HEAP8, ptr, len, offset);
    if (curr < 0) return -1;
    ret += curr;
    if (curr < len) break; // nothing more to read
    if (typeof offset != 'undefined') {
      offset += curr;
    }
  }
  return ret;
};
function _fd_read(fd, iov, iovcnt, pnum) {
  try {
    var stream = SYSCALLS.getStreamFromFD(fd);
    var num = doReadv(stream, iov, iovcnt);
    HEAPU32[pnum >> 2] = num;
    return 0;
  } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return e.errno;
  }
}
var convertI32PairToI53Checked = function convertI32PairToI53Checked(lo, hi) {
  return hi + 0x200000 >>> 0 < 0x400001 - !!lo ? (lo >>> 0) + hi * 4294967296 : NaN;
};
function _fd_seek(fd, offset_low, offset_high, whence, newOffset) {
  var offset = convertI32PairToI53Checked(offset_low, offset_high);
  try {
    if (isNaN(offset)) return 61;
    var stream = SYSCALLS.getStreamFromFD(fd);
    FS.llseek(stream, offset, whence);
    tempI64 = [stream.position >>> 0, (tempDouble = stream.position, +Math.abs(tempDouble) >= 1.0 ? tempDouble > 0.0 ? +Math.floor(tempDouble / 4294967296.0) >>> 0 : ~~+Math.ceil((tempDouble - +(~~tempDouble >>> 0)) / 4294967296.0) >>> 0 : 0)], HEAP32[newOffset >> 2] = tempI64[0], HEAP32[newOffset + 4 >> 2] = tempI64[1];
    if (stream.getdents && offset === 0 && whence === 0) stream.getdents = null; // reset readdir state
    return 0;
  } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return e.errno;
  }
  ;
}

/** @param {number=} offset */
var doWritev = function doWritev(stream, iov, iovcnt, offset) {
  var ret = 0;
  for (var i = 0; i < iovcnt; i++) {
    var ptr = HEAPU32[iov >> 2];
    var len = HEAPU32[iov + 4 >> 2];
    iov += 8;
    var curr = FS.write(stream, HEAP8, ptr, len, offset);
    if (curr < 0) return -1;
    ret += curr;
    if (curr < len) {
      // No more space to write.
      break;
    }
    if (typeof offset != 'undefined') {
      offset += curr;
    }
  }
  return ret;
};
function _fd_write(fd, iov, iovcnt, pnum) {
  try {
    var stream = SYSCALLS.getStreamFromFD(fd);
    var num = doWritev(stream, iov, iovcnt);
    HEAPU32[pnum >> 2] = num;
    return 0;
  } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return e.errno;
  }
}
var stringToUTF8 = function stringToUTF8(str, outPtr, maxBytesToWrite) {
  return stringToUTF8Array(str, HEAPU8, outPtr, maxBytesToWrite);
};
var stackAlloc = function stackAlloc(sz) {
  return _emscripten_stack_alloc(sz);
};
var stringToUTF8OnStack = function stringToUTF8OnStack(str) {
  var size = lengthBytesUTF8(str) + 1;
  var ret = stackAlloc(size);
  stringToUTF8(str, ret, size);
  return ret;
};
var runAndAbortIfError = function runAndAbortIfError(func) {
  try {
    return func();
  } catch (e) {
    abort(e);
  }
};
var sigToWasmTypes = function sigToWasmTypes(sig) {
  var typeNames = {
    'i': 'i32',
    'j': 'i64',
    'f': 'f32',
    'd': 'f64',
    'e': 'externref',
    'p': 'i32'
  };
  var type = {
    parameters: [],
    results: sig[0] == 'v' ? [] : [typeNames[sig[0]]]
  };
  for (var i = 1; i < sig.length; ++i) {
    type.parameters.push(typeNames[sig[i]]);
  }
  return type;
};
var runtimeKeepalivePush = function runtimeKeepalivePush() {
  runtimeKeepaliveCounter += 1;
};
var runtimeKeepalivePop = function runtimeKeepalivePop() {
  runtimeKeepaliveCounter -= 1;
};
var Asyncify = {
  instrumentWasmImports: function instrumentWasmImports(imports) {
    var importPattern = /^(emscripten_sleep|gam_sem_wait|invoke_.*|__asyncjs__.*)$/;
    for (var _i2 = 0, _Object$entries = Object.entries(imports); _i2 < _Object$entries.length; _i2++) {
      var _Object$entries$_i = _slicedToArray(_Object$entries[_i2], 2),
        x = _Object$entries$_i[0],
        original = _Object$entries$_i[1];
      if (typeof original == 'function') {
        var isAsyncifyImport = original.isAsync || importPattern.test(x);
      }
    }
  },
  instrumentWasmExports: function instrumentWasmExports(exports) {
    var ret = {};
    var _loop = function _loop() {
      var _Object$entries2$_i = _slicedToArray(_Object$entries2[_i3], 2),
        x = _Object$entries2$_i[0],
        original = _Object$entries2$_i[1];
      if (typeof original == 'function') {
        ret[x] = function () {
          Asyncify.exportCallStack.push(x);
          try {
            return original.apply(void 0, arguments);
          } finally {
            if (!ABORT) {
              var y = Asyncify.exportCallStack.pop();
              Asyncify.maybeStopUnwind();
            }
          }
        };
      } else {
        ret[x] = original;
      }
    };
    for (var _i3 = 0, _Object$entries2 = Object.entries(exports); _i3 < _Object$entries2.length; _i3++) {
      _loop();
    }
    return ret;
  },
  State: {
    Normal: 0,
    Unwinding: 1,
    Rewinding: 2,
    Disabled: 3
  },
  state: 0,
  StackSize: 4096,
  currData: null,
  handleSleepReturnValue: 0,
  exportCallStack: [],
  callStackNameToId: {},
  callStackIdToName: {},
  callStackId: 0,
  asyncPromiseHandlers: null,
  sleepCallbacks: [],
  getCallStackId: function getCallStackId(funcName) {
    var id = Asyncify.callStackNameToId[funcName];
    if (id === undefined) {
      id = Asyncify.callStackId++;
      Asyncify.callStackNameToId[funcName] = id;
      Asyncify.callStackIdToName[id] = funcName;
    }
    return id;
  },
  maybeStopUnwind: function maybeStopUnwind() {
    if (Asyncify.currData && Asyncify.state === Asyncify.State.Unwinding && Asyncify.exportCallStack.length === 0) {
      // We just finished unwinding.
      // Be sure to set the state before calling any other functions to avoid
      // possible infinite recursion here (For example in debug pthread builds
      // the dbg() function itself can call back into WebAssembly to get the
      // current pthread_self() pointer).
      Asyncify.state = Asyncify.State.Normal;

      // Keep the runtime alive so that a re-wind can be done later.
      runAndAbortIfError(_asyncify_stop_unwind2);
      if (typeof Fibers != 'undefined') {
        Fibers.trampoline();
      }
    }
  },
  whenDone: function whenDone() {
    return new Promise(function (resolve, reject) {
      Asyncify.asyncPromiseHandlers = {
        resolve: resolve,
        reject: reject
      };
    });
  },
  allocateData: function allocateData() {
    // An asyncify data structure has three fields:
    //  0  current stack pos
    //  4  max stack pos
    //  8  id of function at bottom of the call stack (callStackIdToName[id] == name of js function)
    //
    // The Asyncify ABI only interprets the first two fields, the rest is for the runtime.
    // We also embed a stack in the same memory region here, right next to the structure.
    // This struct is also defined as asyncify_data_t in emscripten/fiber.h
    var ptr = _malloc(12 + Asyncify.StackSize);
    Asyncify.setDataHeader(ptr, ptr + 12, Asyncify.StackSize);
    Asyncify.setDataRewindFunc(ptr);
    return ptr;
  },
  setDataHeader: function setDataHeader(ptr, stack, stackSize) {
    HEAPU32[ptr >> 2] = stack;
    HEAPU32[ptr + 4 >> 2] = stack + stackSize;
  },
  setDataRewindFunc: function setDataRewindFunc(ptr) {
    var bottomOfCallStack = Asyncify.exportCallStack[0];
    var rewindId = Asyncify.getCallStackId(bottomOfCallStack);
    HEAP32[ptr + 8 >> 2] = rewindId;
  },
  getDataRewindFuncName: function getDataRewindFuncName(ptr) {
    var id = HEAP32[ptr + 8 >> 2];
    var name = Asyncify.callStackIdToName[id];
    return name;
  },
  getDataRewindFunc: function getDataRewindFunc(name) {
    var func = wasmExports[name];
    return func;
  },
  doRewind: function doRewind(ptr) {
    var name = Asyncify.getDataRewindFuncName(ptr);
    var func = Asyncify.getDataRewindFunc(name);
    // Once we have rewound and the stack we no longer need to artificially
    // keep the runtime alive.

    return func();
  },
  handleSleep: function handleSleep(startAsync) {
    if (ABORT) return;
    if (Asyncify.state === Asyncify.State.Normal) {
      // Prepare to sleep. Call startAsync, and see what happens:
      // if the code decided to call our callback synchronously,
      // then no async operation was in fact begun, and we don't
      // need to do anything.
      var reachedCallback = false;
      var reachedAfterCallback = false;
      startAsync(function () {
        var handleSleepReturnValue = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : 0;
        if (ABORT) return;
        Asyncify.handleSleepReturnValue = handleSleepReturnValue;
        reachedCallback = true;
        if (!reachedAfterCallback) {
          // We are happening synchronously, so no need for async.
          return;
        }
        Asyncify.state = Asyncify.State.Rewinding;
        runAndAbortIfError(function () {
          return _asyncify_start_rewind2(Asyncify.currData);
        });
        if (typeof MainLoop != 'undefined' && MainLoop.func) {
          MainLoop.resume();
        }
        var asyncWasmReturnValue,
          isError = false;
        try {
          asyncWasmReturnValue = Asyncify.doRewind(Asyncify.currData);
        } catch (err) {
          asyncWasmReturnValue = err;
          isError = true;
        }
        // Track whether the return value was handled by any promise handlers.
        var handled = false;
        if (!Asyncify.currData) {
          // All asynchronous execution has finished.
          // `asyncWasmReturnValue` now contains the final
          // return value of the exported async WASM function.
          //
          // Note: `asyncWasmReturnValue` is distinct from
          // `Asyncify.handleSleepReturnValue`.
          // `Asyncify.handleSleepReturnValue` contains the return
          // value of the last C function to have executed
          // `Asyncify.handleSleep()`, where as `asyncWasmReturnValue`
          // contains the return value of the exported WASM function
          // that may have called C functions that
          // call `Asyncify.handleSleep()`.
          var asyncPromiseHandlers = Asyncify.asyncPromiseHandlers;
          if (asyncPromiseHandlers) {
            Asyncify.asyncPromiseHandlers = null;
            (isError ? asyncPromiseHandlers.reject : asyncPromiseHandlers.resolve)(asyncWasmReturnValue);
            handled = true;
          }
        }
        if (isError && !handled) {
          // If there was an error and it was not handled by now, we have no choice but to
          // rethrow that error into the global scope where it can be caught only by
          // `onerror` or `onunhandledpromiserejection`.
          throw asyncWasmReturnValue;
        }
      });
      reachedAfterCallback = true;
      if (!reachedCallback) {
        // A true async operation was begun; start a sleep.
        Asyncify.state = Asyncify.State.Unwinding;
        // TODO: reuse, don't alloc/free every sleep
        Asyncify.currData = Asyncify.allocateData();
        if (typeof MainLoop != 'undefined' && MainLoop.func) {
          MainLoop.pause();
        }
        runAndAbortIfError(function () {
          return _asyncify_start_unwind2(Asyncify.currData);
        });
      }
    } else if (Asyncify.state === Asyncify.State.Rewinding) {
      // Stop a resume.
      Asyncify.state = Asyncify.State.Normal;
      runAndAbortIfError(_asyncify_stop_rewind2);
      _free(Asyncify.currData);
      Asyncify.currData = null;
      // Call all sleep callbacks now that the sleep-resume is all done.
      Asyncify.sleepCallbacks.forEach(callUserCallback);
    } else {
      abort("invalid state: ".concat(Asyncify.state));
    }
    return Asyncify.handleSleepReturnValue;
  },
  handleAsync: function handleAsync(startAsync) {
    return Asyncify.handleSleep(function (wakeUp) {
      // TODO: add error handling as a second param when handleSleep implements it.
      startAsync().then(wakeUp);
    });
  }
};
Module['requestAnimationFrame'] = MainLoop.requestAnimationFrame;
Module['pauseMainLoop'] = MainLoop.pause;
Module['resumeMainLoop'] = MainLoop.resume;
MainLoop.init();
;
FS.createPreloadedFile = FS_createPreloadedFile;
FS.staticInit();
// Set module methods based on EXPORTED_RUNTIME_METHODS
;

// This error may happen quite a bit. To avoid overhead we reuse it (and
// suffer a lack of stack info).
MEMFS.doesNotExistError = new FS.ErrnoError(44);
/** @suppress {checkTypes} */
MEMFS.doesNotExistError.stack = '<generic error, no stack>';
;
// End JS library code

var ASM_CONSTS = {
  2593072: function _($0) {
    console.log('C: 游戏速度已设置为: ' + $0 + '倍');
  },
  2593135: function _() {
    if (window.bayeStart) bayeStart();
  },
  2593174: function _() {
    if (window.bayeExit) bayeExit();
  },
  2593211: function _($0) {
    bayeFlushLcdBuffer($0);
  },
  2593239: function _($0) {
    if (window.lcdSetDotSize) {
      window.lcdSetDotSize($0);
    }
  },
  2593298: function _($0) {
    var key = UTF8ToString($0);
    var value = "";
    var filename = "baye/" + key;
    if (window.bayeLoadFileContent) {
      value = window.bayeLoadFileContent(filename);
    } else {
      value = window.localStorage[filename];
    }
    if (value) {
      var buffer = Module._bayeAlloc(value.length + 1);
      Module.stringToUTF8(value, buffer, value.length + 1);
      return buffer;
    }
    return 0;
  },
  2593646: function _($0, $1) {
    var key = UTF8ToString($0);
    var value = UTF8ToString($1);
    var filename = "baye/" + key;
    if (window.bayeSaveFileContent) {
      window.bayeSaveFileContent(filename, value);
    } else {
      window.localStorage[filename] = value;
    }
  },
  2593867: function _() {
    if (window.baye == undefined) {
      window.baye = {};
    }
    if (window.baye.hooks == undefined) {
      window.baye.hooks = {};
    }
    window.baye.data = baye_bridge_value(_bayeGetGlobal());
  },
  2594043: function _($0) {
    if (window.baye.preScriptInit) {
      window.baye.preScriptInit();
    }
    var script = UTF8ToString($0);
    eval(script);
  },
  2594156: function _($0, $1) {
    var name = UTF8ToString($0);
    var rv = 0;
    if (window.baye == undefined || window.baye.hooks == undefined || window.baye.hooks[name] == undefined) {
      rv = -1;
    } else {
      var cContext = $1;
      if (cContext != 0) {
        var jsContext = baye_bridge_value(cContext);
        rv = baye.callHook(name, jsContext);
      } else {
        rv = baye.callHook(name, undefined);
      }
    }
    return rv;
  },
  2594508: function _($0) {
    var name = UTF8ToString($0);
    if (window.baye && window.baye.hooks && window.baye.hooks[name]) {
      return 1;
    } else {
      return 0;
    }
  },
  2594639: function _($0) {
    var scr = UTF8ToString($0);
    eval(scr);
  },
  2594682: function _($0) {
    return baye.callCallback($0);
  },
  2594716: function _() {
    var now = new Date();
    return now.getTime();
  }
};
function gam_sem_create() {
  if (!Module.sems) {
    Module.sems = [];
  }
  for (var i = 0;; i++) {
    if (Module.sems[i] == undefined) {
      Module.sems[i] = {
        cnt: 0
      };
      return i;
    }
  }
}
function gam_sem_delete(semid) {
  Module.sems[semid] = undefined;
}
function gam_sem_signal(semid) {
  var sem = Module.sems[semid];
  sem.cnt += 1;
  if (sem.signal) {
    sem.signal();
    sem.signal = null;
  }
}
function gam_sem_wait(semid) {
  return Asyncify.handleSleep(function (wakeUp) {
    var sem = Module.sems[semid];
    function signal() {
      sem.cnt -= 1;
      setTimeout(wakeUp, 0);
    }
    if (sem.cnt > 0) {
      signal();
    } else {
      sem.signal = signal;
    }
  });
}
var wasmImports = {
  /** @export */
  _abort_js: __abort_js,
  /** @export */
  emscripten_asm_const_int: _emscripten_asm_const_int,
  /** @export */
  emscripten_async_call: _emscripten_async_call,
  /** @export */
  emscripten_resize_heap: _emscripten_resize_heap,
  /** @export */
  emscripten_sleep: _emscripten_sleep,
  /** @export */
  exit: _exit,
  /** @export */
  fd_close: _fd_close,
  /** @export */
  fd_read: _fd_read,
  /** @export */
  fd_seek: _fd_seek,
  /** @export */
  fd_write: _fd_write,
  /** @export */
  gam_sem_create: gam_sem_create,
  /** @export */
  gam_sem_signal: gam_sem_signal,
  /** @export */
  gam_sem_wait: gam_sem_wait
};
var wasmExports;
createWasm();
var _wasm_call_ctors = function ___wasm_call_ctors() {
  return (_wasm_call_ctors = wasmExports['__wasm_call_ctors'])();
};
var _bayeSendKey = Module['_bayeSendKey'] = function (a0) {
  return (_bayeSendKey = Module['_bayeSendKey'] = wasmExports['bayeSendKey'])(a0);
};
var _bayeSendTouchEvent = Module['_bayeSendTouchEvent'] = function (a0, a1, a2) {
  return (_bayeSendTouchEvent = Module['_bayeSendTouchEvent'] = wasmExports['bayeSendTouchEvent'])(a0, a1, a2);
};
var _bayeSetLcdSize = Module['_bayeSetLcdSize'] = function (a0, a1) {
  return (_bayeSetLcdSize = Module['_bayeSetLcdSize'] = wasmExports['bayeSetLcdSize'])(a0, a1);
};
var _bayeSetDebug = Module['_bayeSetDebug'] = function (a0) {
  return (_bayeSetDebug = Module['_bayeSetDebug'] = wasmExports['bayeSetDebug'])(a0);
};
var _bayeGetCurrentPeriod = Module['_bayeGetCurrentPeriod'] = function () {
  return (_bayeGetCurrentPeriod = Module['_bayeGetCurrentPeriod'] = wasmExports['bayeGetCurrentPeriod'])();
};
var _bayeCityAddGoods = Module['_bayeCityAddGoods'] = function (a0, a1) {
  return (_bayeCityAddGoods = Module['_bayeCityAddGoods'] = wasmExports['bayeCityAddGoods'])(a0, a1);
};
var _bayeCityDelGoods = Module['_bayeCityDelGoods'] = function (a0, a1) {
  return (_bayeCityDelGoods = Module['_bayeCityDelGoods'] = wasmExports['bayeCityDelGoods'])(a0, a1);
};
var _Value_get_type = Module['_Value_get_type'] = function (a0) {
  return (_Value_get_type = Module['_Value_get_type'] = wasmExports['Value_get_type'])(a0);
};
var _Value_get_def = Module['_Value_get_def'] = function (a0) {
  return (_Value_get_def = Module['_Value_get_def'] = wasmExports['Value_get_def'])(a0);
};
var _Value_get_addr = Module['_Value_get_addr'] = function (a0) {
  return (_Value_get_addr = Module['_Value_get_addr'] = wasmExports['Value_get_addr'])(a0);
};
var _Value_get_u8_value = Module['_Value_get_u8_value'] = function (a0) {
  return (_Value_get_u8_value = Module['_Value_get_u8_value'] = wasmExports['Value_get_u8_value'])(a0);
};
var _Value_set_u8_value = Module['_Value_set_u8_value'] = function (a0, a1) {
  return (_Value_set_u8_value = Module['_Value_set_u8_value'] = wasmExports['Value_set_u8_value'])(a0, a1);
};
var _Value_get_u16_value = Module['_Value_get_u16_value'] = function (a0) {
  return (_Value_get_u16_value = Module['_Value_get_u16_value'] = wasmExports['Value_get_u16_value'])(a0);
};
var _Value_set_u16_value = Module['_Value_set_u16_value'] = function (a0, a1) {
  return (_Value_set_u16_value = Module['_Value_set_u16_value'] = wasmExports['Value_set_u16_value'])(a0, a1);
};
var _Value_get_u32_value = Module['_Value_get_u32_value'] = function (a0) {
  return (_Value_get_u32_value = Module['_Value_get_u32_value'] = wasmExports['Value_get_u32_value'])(a0);
};
var _Value_set_u32_value = Module['_Value_set_u32_value'] = function (a0, a1) {
  return (_Value_set_u32_value = Module['_Value_set_u32_value'] = wasmExports['Value_set_u32_value'])(a0, a1);
};
var _Object_get_field_count = Module['_Object_get_field_count'] = function (a0) {
  return (_Object_get_field_count = Module['_Object_get_field_count'] = wasmExports['Object_get_field_count'])(a0);
};
var _Object_get_field_by_index = Module['_Object_get_field_by_index'] = function (a0, a1) {
  return (_Object_get_field_by_index = Module['_Object_get_field_by_index'] = wasmExports['Object_get_field_by_index'])(a0, a1);
};
var _Field_get_name = Module['_Field_get_name'] = function (a0) {
  return (_Field_get_name = Module['_Field_get_name'] = wasmExports['Field_get_name'])(a0);
};
var _Field_get_value = Module['_Field_get_value'] = function (a0) {
  return (_Field_get_value = Module['_Field_get_value'] = wasmExports['Field_get_value'])(a0);
};
var _Field_get_type = Module['_Field_get_type'] = function (a0) {
  return (_Field_get_type = Module['_Field_get_type'] = wasmExports['Field_get_type'])(a0);
};
var _ValueDef_get_type = Module['_ValueDef_get_type'] = function (a0) {
  return (_ValueDef_get_type = Module['_ValueDef_get_type'] = wasmExports['ValueDef_get_type'])(a0);
};
var _baye_get_u8_value = Module['_baye_get_u8_value'] = function (a0) {
  return (_baye_get_u8_value = Module['_baye_get_u8_value'] = wasmExports['baye_get_u8_value'])(a0);
};
var _baye_set_u8_value = Module['_baye_set_u8_value'] = function (a0, a1) {
  return (_baye_set_u8_value = Module['_baye_set_u8_value'] = wasmExports['baye_set_u8_value'])(a0, a1);
};
var _baye_get_u16_value = Module['_baye_get_u16_value'] = function (a0) {
  return (_baye_get_u16_value = Module['_baye_get_u16_value'] = wasmExports['baye_get_u16_value'])(a0);
};
var _baye_set_u16_value = Module['_baye_set_u16_value'] = function (a0, a1) {
  return (_baye_set_u16_value = Module['_baye_set_u16_value'] = wasmExports['baye_set_u16_value'])(a0, a1);
};
var _baye_get_u32_value = Module['_baye_get_u32_value'] = function (a0) {
  return (_baye_get_u32_value = Module['_baye_get_u32_value'] = wasmExports['baye_get_u32_value'])(a0);
};
var _baye_set_u32_value = Module['_baye_set_u32_value'] = function (a0, a1) {
  return (_baye_set_u32_value = Module['_baye_set_u32_value'] = wasmExports['baye_set_u32_value'])(a0, a1);
};
var _ValueDef_get_field_count = Module['_ValueDef_get_field_count'] = function (a0) {
  return (_ValueDef_get_field_count = Module['_ValueDef_get_field_count'] = wasmExports['ValueDef_get_field_count'])(a0);
};
var _ValueDef_get_field_by_index = Module['_ValueDef_get_field_by_index'] = function (a0, a1) {
  return (_ValueDef_get_field_by_index = Module['_ValueDef_get_field_by_index'] = wasmExports['ValueDef_get_field_by_index'])(a0, a1);
};
var _ValueDef_get_array_length = Module['_ValueDef_get_array_length'] = function (a0) {
  return (_ValueDef_get_array_length = Module['_ValueDef_get_array_length'] = wasmExports['ValueDef_get_array_length'])(a0);
};
var _ValueDef_get_size = Module['_ValueDef_get_size'] = function (a0) {
  return (_ValueDef_get_size = Module['_ValueDef_get_size'] = wasmExports['ValueDef_get_size'])(a0);
};
var _ValueDef_get_array_subdef = Module['_ValueDef_get_array_subdef'] = function (a0) {
  return (_ValueDef_get_array_subdef = Module['_ValueDef_get_array_subdef'] = wasmExports['ValueDef_get_array_subdef'])(a0);
};
var _bayeGetPersonName = Module['_bayeGetPersonName'] = function (a0) {
  return (_bayeGetPersonName = Module['_bayeGetPersonName'] = wasmExports['bayeGetPersonName'])(a0);
};
var _bayeGetToolName = Module['_bayeGetToolName'] = function (a0) {
  return (_bayeGetToolName = Module['_bayeGetToolName'] = wasmExports['bayeGetToolName'])(a0);
};
var _bayeGetSkillName = Module['_bayeGetSkillName'] = function (a0) {
  return (_bayeGetSkillName = Module['_bayeGetSkillName'] = wasmExports['bayeGetSkillName'])(a0);
};
var _bayeGetCityName = Module['_bayeGetCityName'] = function (a0) {
  return (_bayeGetCityName = Module['_bayeGetCityName'] = wasmExports['bayeGetCityName'])(a0);
};
var _bayeStrLen = Module['_bayeStrLen'] = function (a0) {
  return (_bayeStrLen = Module['_bayeStrLen'] = wasmExports['bayeStrLen'])(a0);
};
var _bayeGetGlobal = Module['_bayeGetGlobal'] = function () {
  return (_bayeGetGlobal = Module['_bayeGetGlobal'] = wasmExports['bayeGetGlobal'])();
};
var _bayeGetCustomData = Module['_bayeGetCustomData'] = function () {
  return (_bayeGetCustomData = Module['_bayeGetCustomData'] = wasmExports['bayeGetCustomData'])();
};
var _bayeSetCustomData = Module['_bayeSetCustomData'] = function (a0) {
  return (_bayeSetCustomData = Module['_bayeSetCustomData'] = wasmExports['bayeSetCustomData'])(a0);
};
var _bayeScriptInit = Module['_bayeScriptInit'] = function () {
  return (_bayeScriptInit = Module['_bayeScriptInit'] = wasmExports['bayeScriptInit'])();
};
var _bayeRand = Module['_bayeRand'] = function () {
  return (_bayeRand = Module['_bayeRand'] = wasmExports['bayeRand'])();
};
var _bayeSRand = Module['_bayeSRand'] = function (a0) {
  return (_bayeSRand = Module['_bayeSRand'] = wasmExports['bayeSRand'])(a0);
};
var _bayeGetSeed = Module['_bayeGetSeed'] = function () {
  return (_bayeGetSeed = Module['_bayeGetSeed'] = wasmExports['bayeGetSeed'])();
};
var _bayeLevelUp = Module['_bayeLevelUp'] = function (a0) {
  return (_bayeLevelUp = Module['_bayeLevelUp'] = wasmExports['bayeLevelUp'])(a0);
};
var _bayeOrderNeedMoney = Module['_bayeOrderNeedMoney'] = function (a0) {
  return (_bayeOrderNeedMoney = Module['_bayeOrderNeedMoney'] = wasmExports['bayeOrderNeedMoney'])(a0);
};
var _bayeOrderComsumeMoney = Module['_bayeOrderComsumeMoney'] = function (a0, a1) {
  return (_bayeOrderComsumeMoney = Module['_bayeOrderComsumeMoney'] = wasmExports['bayeOrderComsumeMoney'])(a0, a1);
};
var _bayeFgtGetGenTer = Module['_bayeFgtGetGenTer'] = function (a0) {
  return (_bayeFgtGetGenTer = Module['_bayeFgtGetGenTer'] = wasmExports['bayeFgtGetGenTer'])(a0);
};
var _bayePutPersonInCity = Module['_bayePutPersonInCity'] = function (a0, a1) {
  return (_bayePutPersonInCity = Module['_bayePutPersonInCity'] = wasmExports['bayePutPersonInCity'])(a0, a1);
};
var _bayePutToolInCity = Module['_bayePutToolInCity'] = function (a0, a1, a2) {
  return (_bayePutToolInCity = Module['_bayePutToolInCity'] = wasmExports['bayePutToolInCity'])(a0, a1, a2);
};
var _bayeDeletePersonInCity = Module['_bayeDeletePersonInCity'] = function (a0, a1) {
  return (_bayeDeletePersonInCity = Module['_bayeDeletePersonInCity'] = wasmExports['bayeDeletePersonInCity'])(a0, a1);
};
var _bayeDeleteToolInCity = Module['_bayeDeleteToolInCity'] = function (a0, a1) {
  return (_bayeDeleteToolInCity = Module['_bayeDeleteToolInCity'] = wasmExports['bayeDeleteToolInCity'])(a0, a1);
};
var _bayeGetArmType = Module['_bayeGetArmType'] = function (a0) {
  return (_bayeGetArmType = Module['_bayeGetArmType'] = wasmExports['bayeGetArmType'])(a0);
};
var _bayeGetGBKBuffer = Module['_bayeGetGBKBuffer'] = function () {
  return (_bayeGetGBKBuffer = Module['_bayeGetGBKBuffer'] = wasmExports['bayeGetGBKBuffer'])();
};
var _bayeLcdDrawImage = Module['_bayeLcdDrawImage'] = function (a0, a1, a2, a3, a4, a5) {
  return (_bayeLcdDrawImage = Module['_bayeLcdDrawImage'] = wasmExports['bayeLcdDrawImage'])(a0, a1, a2, a3, a4, a5);
};
var _bayeLcdDrawText = Module['_bayeLcdDrawText'] = function (a0, a1, a2, a3) {
  return (_bayeLcdDrawText = Module['_bayeLcdDrawText'] = wasmExports['bayeLcdDrawText'])(a0, a1, a2, a3);
};
var _bayeLcdClearRect = Module['_bayeLcdClearRect'] = function (a0, a1, a2, a3, a4) {
  return (_bayeLcdClearRect = Module['_bayeLcdClearRect'] = wasmExports['bayeLcdClearRect'])(a0, a1, a2, a3, a4);
};
var _bayeLcdRevertRect = Module['_bayeLcdRevertRect'] = function (a0, a1, a2, a3, a4) {
  return (_bayeLcdRevertRect = Module['_bayeLcdRevertRect'] = wasmExports['bayeLcdRevertRect'])(a0, a1, a2, a3, a4);
};
var _bayeLcdDot = Module['_bayeLcdDot'] = function (a0, a1, a2, a3) {
  return (_bayeLcdDot = Module['_bayeLcdDot'] = wasmExports['bayeLcdDot'])(a0, a1, a2, a3);
};
var _bayeLcdDrawLine = Module['_bayeLcdDrawLine'] = function (a0, a1, a2, a3, a4, a5) {
  return (_bayeLcdDrawLine = Module['_bayeLcdDrawLine'] = wasmExports['bayeLcdDrawLine'])(a0, a1, a2, a3, a4, a5);
};
var _bayeLcdDrawRect = Module['_bayeLcdDrawRect'] = function (a0, a1, a2, a3, a4, a5) {
  return (_bayeLcdDrawRect = Module['_bayeLcdDrawRect'] = wasmExports['bayeLcdDrawRect'])(a0, a1, a2, a3, a4, a5);
};
var _bayeGameEnvInit = Module['_bayeGameEnvInit'] = function () {
  return (_bayeGameEnvInit = Module['_bayeGameEnvInit'] = wasmExports['bayeGameEnvInit'])();
};
var _bayeSaveScreen = Module['_bayeSaveScreen'] = function () {
  return (_bayeSaveScreen = Module['_bayeSaveScreen'] = wasmExports['bayeSaveScreen'])();
};
var _bayeRestoreScreen = Module['_bayeRestoreScreen'] = function () {
  return (_bayeRestoreScreen = Module['_bayeRestoreScreen'] = wasmExports['bayeRestoreScreen'])();
};
var _bayeGetPersonCount = Module['_bayeGetPersonCount'] = function () {
  return (_bayeGetPersonCount = Module['_bayeGetPersonCount'] = wasmExports['bayeGetPersonCount'])();
};
var _bayeAlloc = Module['_bayeAlloc'] = function (a0) {
  return (_bayeAlloc = Module['_bayeAlloc'] = wasmExports['bayeAlloc'])(a0);
};
var _bayePrintGC = Module['_bayePrintGC'] = function () {
  return (_bayePrintGC = Module['_bayePrintGC'] = wasmExports['bayePrintGC'])();
};
var _bayeGCCheckAll = Module['_bayeGCCheckAll'] = function () {
  return (_bayeGCCheckAll = Module['_bayeGCCheckAll'] = wasmExports['bayeGCCheckAll'])();
};
var _bayeLoadPeriod = Module['_bayeLoadPeriod'] = function (a0) {
  return (_bayeLoadPeriod = Module['_bayeLoadPeriod'] = wasmExports['bayeLoadPeriod'])(a0);
};
var _bayeSetFont = Module['_bayeSetFont'] = function (a0) {
  return (_bayeSetFont = Module['_bayeSetFont'] = wasmExports['bayeSetFont'])(a0);
};
var _bayeSetFontEn = Module['_bayeSetFontEn'] = function (a0) {
  return (_bayeSetFontEn = Module['_bayeSetFontEn'] = wasmExports['bayeSetFontEn'])(a0);
};
var _bayeClearFontCache = Module['_bayeClearFontCache'] = function () {
  return (_bayeClearFontCache = Module['_bayeClearFontCache'] = wasmExports['bayeClearFontCache'])();
};
var _bayeGetSpeed = Module['_bayeGetSpeed'] = function () {
  return (_bayeGetSpeed = Module['_bayeGetSpeed'] = wasmExports['bayeGetSpeed'])();
};
var _bayeSetSpeed = Module['_bayeSetSpeed'] = function (a0) {
  return (_bayeSetSpeed = Module['_bayeSetSpeed'] = wasmExports['bayeSetSpeed'])(a0);
};
var _main = Module['_main'] = function (a0, a1) {
  return (_main = Module['_main'] = wasmExports['__main_argc_argv'])(a0, a1);
};
var _free = Module['_free'] = function (a0) {
  return (_free = Module['_free'] = wasmExports['free'])(a0);
};
var _SetAgricultureMultiplier = Module['_SetAgricultureMultiplier'] = function (a0) {
  return (_SetAgricultureMultiplier = Module['_SetAgricultureMultiplier'] = wasmExports['SetAgricultureMultiplier'])(a0);
};
var _SetCommerceMultiplier = Module['_SetCommerceMultiplier'] = function (a0) {
  return (_SetCommerceMultiplier = Module['_SetCommerceMultiplier'] = wasmExports['SetCommerceMultiplier'])(a0);
};
var _malloc = Module['_malloc'] = function (a0) {
  return (_malloc = Module['_malloc'] = wasmExports['malloc'])(a0);
};
var _emscripten_tempret_set = function __emscripten_tempret_set(a0) {
  return (_emscripten_tempret_set = wasmExports['_emscripten_tempret_set'])(a0);
};
var _emscripten_stack_restore = function __emscripten_stack_restore(a0) {
  return (_emscripten_stack_restore = wasmExports['_emscripten_stack_restore'])(a0);
};
var _emscripten_stack_alloc = function __emscripten_stack_alloc(a0) {
  return (_emscripten_stack_alloc = wasmExports['_emscripten_stack_alloc'])(a0);
};
var _emscripten_stack_get_current2 = function _emscripten_stack_get_current() {
  return (_emscripten_stack_get_current2 = wasmExports['emscripten_stack_get_current'])();
};
var dynCall_vi = Module['dynCall_vi'] = function (a0, a1) {
  return (dynCall_vi = Module['dynCall_vi'] = wasmExports['dynCall_vi'])(a0, a1);
};
var dynCall_iii = Module['dynCall_iii'] = function (a0, a1, a2) {
  return (dynCall_iii = Module['dynCall_iii'] = wasmExports['dynCall_iii'])(a0, a1, a2);
};
var dynCall_iiii = Module['dynCall_iiii'] = function (a0, a1, a2, a3) {
  return (dynCall_iiii = Module['dynCall_iiii'] = wasmExports['dynCall_iiii'])(a0, a1, a2, a3);
};
var dynCall_vii = Module['dynCall_vii'] = function (a0, a1, a2) {
  return (dynCall_vii = Module['dynCall_vii'] = wasmExports['dynCall_vii'])(a0, a1, a2);
};
var dynCall_v = Module['dynCall_v'] = function (a0) {
  return (dynCall_v = Module['dynCall_v'] = wasmExports['dynCall_v'])(a0);
};
var dynCall_ii = Module['dynCall_ii'] = function (a0, a1) {
  return (dynCall_ii = Module['dynCall_ii'] = wasmExports['dynCall_ii'])(a0, a1);
};
var dynCall_iiiii = Module['dynCall_iiiii'] = function (a0, a1, a2, a3, a4) {
  return (dynCall_iiiii = Module['dynCall_iiiii'] = wasmExports['dynCall_iiiii'])(a0, a1, a2, a3, a4);
};
var dynCall_jiji = Module['dynCall_jiji'] = function (a0, a1, a2, a3, a4) {
  return (dynCall_jiji = Module['dynCall_jiji'] = wasmExports['dynCall_jiji'])(a0, a1, a2, a3, a4);
};
var dynCall_iidiiii = Module['dynCall_iidiiii'] = function (a0, a1, a2, a3, a4, a5, a6) {
  return (dynCall_iidiiii = Module['dynCall_iidiiii'] = wasmExports['dynCall_iidiiii'])(a0, a1, a2, a3, a4, a5, a6);
};
var _asyncify_start_unwind2 = function _asyncify_start_unwind(a0) {
  return (_asyncify_start_unwind2 = wasmExports['asyncify_start_unwind'])(a0);
};
var _asyncify_stop_unwind2 = function _asyncify_stop_unwind() {
  return (_asyncify_stop_unwind2 = wasmExports['asyncify_stop_unwind'])();
};
var _asyncify_start_rewind2 = function _asyncify_start_rewind(a0) {
  return (_asyncify_start_rewind2 = wasmExports['asyncify_start_rewind'])(a0);
};
var _asyncify_stop_rewind2 = function _asyncify_stop_rewind() {
  return (_asyncify_stop_rewind2 = wasmExports['asyncify_stop_rewind'])();
};

// include: postamble.js
// === Auto-generated postamble setup entry stuff ===

Module['setValue'] = setValue;
Module['getValue'] = getValue;
Module['stringToUTF8'] = stringToUTF8;
function callMain() {
  var args = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : [];
  var entryFunction = _main;
  args.unshift(thisProgram);
  var argc = args.length;
  var argv = stackAlloc((argc + 1) * 4);
  var argv_ptr = argv;
  args.forEach(function (arg) {
    HEAPU32[argv_ptr >> 2] = stringToUTF8OnStack(arg);
    argv_ptr += 4;
  });
  HEAPU32[argv_ptr >> 2] = 0;
  try {
    var ret = entryFunction(argc, argv);

    // if we're not running an evented main loop, it's time to exit
    exitJS(ret, /* implicit = */true);
    return ret;
  } catch (e) {
    return handleException(e);
  }
}
function run() {
  var args = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : arguments_;
  if (runDependencies > 0) {
    dependenciesFulfilled = run;
    return;
  }
  preRun();

  // a preRun added a dependency, run will be called later
  if (runDependencies > 0) {
    dependenciesFulfilled = run;
    return;
  }
  function doRun() {
    var _Module$onRuntimeInit;
    // run may have just been called through dependencies being fulfilled just in this very frame,
    // or while the async setStatus time below was happening
    Module['calledRun'] = true;
    if (ABORT) return;
    initRuntime();
    preMain();
    (_Module$onRuntimeInit = Module['onRuntimeInitialized']) === null || _Module$onRuntimeInit === void 0 || _Module$onRuntimeInit.call(Module);
    var noInitialRun = Module['noInitialRun'];
    if (!noInitialRun) callMain(args);
    postRun();
  }
  if (Module['setStatus']) {
    Module['setStatus']('Running...');
    setTimeout(function () {
      setTimeout(function () {
        return Module['setStatus']('');
      }, 1);
      doRun();
    }, 1);
  } else {
    doRun();
  }
}
if (Module['preInit']) {
  if (typeof Module['preInit'] == 'function') Module['preInit'] = [Module['preInit']];
  while (Module['preInit'].length > 0) {
    Module['preInit'].pop()();
  }
}
run();

// end include: postamble.js
